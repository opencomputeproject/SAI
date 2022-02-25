import os
from subprocess import call, Popen, PIPE
import re
import os
import sys
import json
from datetime import datetime
from shutil import copy2
from glob import glob
import P4_aux as aux


def api_set_lib_paths(lib, template_dir):
		lib.h_template_path = os.path.join(template_dir, 'sai_lib_template.h')
		lib.name = lib.name.split('lib')[-1]
		lib.inc_path = os.path.join(lib.output_path, 'sai_inc')
		lib.h_path = os.path.join(lib.inc_path, 'sai' + lib.name + '.h')
		lib.src_path = os.path.join(lib.output_path, 'sai_src')
		lib.c_path = os.path.join(lib.src_path, 'libsai_'+lib.name +'.c')

#----------------------------------------------------------------------------
# header files gen

def sai_write_table_id_enum(lib):
	enum_txt =''
	for table in lib.tables:
		enum_txt += '/** SAI extension table {name} in pipe {pipe}*/\n'.format(name = table.cname,pipe=table.flexPipe)
		enum_txt += '	SAI_{pipe}_{name},\n\n'.format(name = table.cname.upper(),pipe=table.flexPipe.upper())
	return enum_txt

def create_header(brief='', type='', flags='', objects='', condition='', params_out=[], params_in=[], params_inout=[], return_='', tabs=0, isvlan=0, default=''):
	hdr = '    '*tabs + '/**\n' + '    '*tabs + ' * @brief %s\n' % brief
	if (type or flags or objects or condition or params_out or params_in or return_):
		hdr += '    '*tabs + ' *\n'
	if type:
		hdr += '    '*tabs + ' * @type %s\n' % type
	if flags:
		hdr += '    '*tabs + ' * @flags %s\n' % flags
	if default:
		hdr += '    '*tabs + ' * @default %s\n' % default
	if isvlan == 1:
		hdr += '    '*tabs + ' * @isvlan false\n'
	if isvlan == 2:
		hdr += '    '*tabs + ' * @isvlan true\n'
	if objects:
		hdr += '    '*tabs + ' * @objects %s\n' % objects
	if condition:
		hdr += '    '*tabs + ' * @condition %s\n' % condition
	for param in params_in:
		hdr += '    '*tabs + ' * @param[in] %s\n' % param
	for param in params_out:
		hdr += '    '*tabs + ' * @param[out] %s\n' % param
	for param in params_inout:
		hdr += '    '*tabs + ' * @param[inout] %s\n' % param
	if return_:
		hdr += '    '*tabs + ' *\n' + '    '*tabs + ' * @return %s\n' % return_

	hdr += '    '*tabs + ' */\n'
	return hdr

def sai_create_action_type_enum(table):
	enum_txt = create_header(brief='Attribute data for #SAI_%s_ENTRY_ATTR_ACTION' % table.cname.upper())
	enum_txt+='typedef enum _sai_%s_entry_action_t\n{\n'%table.cname.lower()
	# enum_txt+='    SAI_%s_ENTRY_ACTION_NO_ACTION,\n\n' % table.cname.upper()
	for action_name,action_id in zip(table.cactions,table.action_ids):
		# enum_txt+='    /** upon table entry hit, invoke action %s */\n'%action_name
		# if action_name != 'NoAction':
		enum_txt+='    SAI_%s_ENTRY_ACTION_%s,\n\n' % (table.cname.upper(), action_name.upper())
	enum_txt+='} sai_%s_entry_action_t;\n\n'%table.cname
	return enum_txt

def get_sai_key(lib, key):
	sai_key_dict = lib.sai_keys[key]
	if 'sai_object_type' in sai_key_dict:
		sai_object_type = sai_key_dict['sai_object_type']
	else:
		sai_object_type = ''
	return sai_key_dict['sai_name'], sai_key_dict['sai_type'], sai_object_type

def sai_write_table_action_enum(lib):
	enum_txt=''
	for table in lib.tables:
		enum_txt+=sai_create_action_type_enum(table)
	return enum_txt

def get_action_def(lib, action_id):
	for action_def in lib.p4_action_def:
		if action_def['id'] == action_id:
			return action_def
	return None

def sai_write_table_attr(lib):
	enum_txt=''
	for table in lib.tables:
		attr_prefix = 'SAI_%s_ENTRY_ATTR' % table.cname.upper()
		enum_txt += create_header(brief=('Attribute ID for %s' % table.cname))
		enum_txt += 'typedef enum _sai_%s_entry_attr_t\n{\n' % (table.cname)
		enum_txt += create_header(brief='Start of attributes', tabs=1)
		enum_txt += '    %s_START,\n\n' % attr_prefix
		enum_txt += create_header(brief = 'Action', type='sai_%s_entry_action_t' % table.cname, flags='MANDATORY_ON_CREATE | CREATE_ONLY', tabs=1)
		enum_txt += '    %s_ACTION = %s_START,\n\n' % (attr_prefix, attr_prefix)
		if ('ternary' in table.key_types):
			enum_txt += create_header(brief = 'Rule priority in table', type='sai_uint32_t', flags='MANDATORY_ON_CREATE | CREATE_ONLY', tabs=1)
			enum_txt += '    %s_PRIORITY,\n\n' % attr_prefix
		for key_type, key in zip(table.key_types,table.key_fields):
			sai_key_name, sai_key_type, sai_object_type = get_sai_key(lib, key)
			isvlan = 0
			if sai_key_type == 'sai_uint16_t':
				isvlan = 1 #TODO check for 2
			if key_type == 'exact':
				enum_txt += create_header(brief = 'Matched key %s' % sai_key_name, type=sai_key_type, objects=sai_object_type, flags='MANDATORY_ON_CREATE | CREATE_ONLY', tabs=1, isvlan=isvlan)
				enum_txt += '    %s_%s,\n\n' % (attr_prefix, sai_key_name.upper())
			if key_type == 'ternary':
				enum_txt += create_header(brief = 'Matched key %s (key)' % sai_key_name, type=sai_key_type, objects=sai_object_type, flags='MANDATORY_ON_CREATE | CREATE_ONLY', tabs=1, isvlan=isvlan)
				enum_txt += '    %s_%s_KEY,\n\n' % (attr_prefix, sai_key_name.upper())
				enum_txt += create_header(brief = 'Matched key %s (mask)' % sai_key_name, type=sai_key_type, objects=sai_object_type, flags='MANDATORY_ON_CREATE | CREATE_ONLY', tabs=1, isvlan=isvlan)
				enum_txt += '    %s_%s_MASK,\n\n' % (attr_prefix, sai_key_name.upper())
		if ('exact' in table.key_types):
			enum_txt += create_header(brief = 'Is default entry', type='bool', default='false', flags='CREATE_ONLY', tabs=1)
			enum_txt += '    %s_IS_DEFAULT,\n\n' % attr_prefix
		for action_name, action_id in zip(table.cactions, table.action_ids):	
			action_def = get_action_def(lib, action_id)
			if action_def['primitives']:
				for primitive in action_def['primitives']:
					op = primitive['op']
					# if op == 'hit_counter': # TODO Counter
						# enum_txt += create_header(brief='Action %s hit counter' % action_name, flags='CREATE_AND_SET', type='sai_object_id_t', objects='SAI_OBJECT_TYPE_COUNTER', tabs=1)
						# enum_txt += '    %s_%s_COUNTER,\n\n' % (attr_prefix, action_name.upper())
					sai_action = lib.sai_actions[op]
					if 'sai_params' in sai_action:
						for sai_param in sai_action['sai_params']:
							isvlan = 0 # 0 - no tag, 1 - tag false, 2 - tag true
							if sai_param['type'] == 'sai_uint16_t':
								isvlan = 1 # TODO: add check if needs vlan true
							enum_txt += create_header(brief='Action %s parameter %s' % (action_name, sai_param['name']),
													  type=sai_param['type'],
													  objects=sai_param['object_type'],
													  condition='%s_ACTION == SAI_%s_ENTRY_ACTION_%s' % (attr_prefix, table.cname.upper(), action_name.upper()),
													  flags='MANDATORY_ON_CREATE | CREATE_ONLY',
													  tabs=1,
													  isvlan=isvlan) 
							enum_txt += '    %s_%s,\n\n' % (attr_prefix, sai_param['name'].upper())
		enum_txt += create_header(brief='End of attributes', tabs=1)
		enum_txt += '    %s_END,\n\n' % attr_prefix
		enum_txt += '    /** Custom range base value */\n'
		enum_txt += '    %s_CUSTOM_RANGE_START = 0x10000000,\n\n' % attr_prefix
		enum_txt += '    /** End of custom range base */\n'
		enum_txt += '    %s_CUSTOM_RANGE_END,\n\n' % attr_prefix
		enum_txt += '} sai_%s_entry_attr_t;\n\n' % (table.cname)
	return enum_txt

def sai_write_table_api_fn(lib):
	enum_txt = ''
	for table in lib.tables:
		enum_txt += '    sai_create_%s_entry_fn            create_%s_entry;\n' % (table.cname, table.cname)
		enum_txt += '    sai_remove_%s_entry_fn            remove_%s_entry;\n' % (table.cname, table.cname)
		enum_txt += '    sai_set_%s_entry_attribute_fn    set_%s_entry_attribute;\n' % (table.cname, table.cname)
		enum_txt += '    sai_get_%s_entry_attribute_fn    get_%s_entry_attribute;\n' % (table.cname, table.cname)
	enum_txt += '    sai_get_%s_stats_fn    get_%s_stats;\n' % (lib.name, lib.name)
	enum_txt += '    sai_clear_%s_stats_fn    clear_%s_stats;\n' % (lib.name, lib.name)
	return enum_txt

def sai_write_table_fn_def(lib):
	enum_txt = ''
	for table in lib.tables:
		enum_txt += create_header(brief='Create %s_entry' % table.cname,
							      params_out=['entry_id Entry id'], 
							      params_in=['switch_id Switch id', 'attr_count Number of attributes', 'attr_list Array of attributes'], 
							      return_='#SAI_STATUS_SUCCESS on success Failure status code on error')
		enum_txt += 'typedef sai_status_t(*sai_create_%s_entry_fn)(\n        _Out_ sai_object_id_t *entry_id,\n        _In_ sai_object_id_t switch_id,\n        _In_ uint32_t attr_count,\n        _In_ const sai_attribute_t *attr_list);\n\n' % table.cname
		enum_txt += create_header(brief='Remove %s_entry' % table.cname, 
								  params_in=['entry_id Entry id'], 
								  return_='#SAI_STATUS_SUCCESS on success Failure status code on error')
		enum_txt += 'typedef sai_status_t(*sai_remove_%s_entry_fn)(\n        _In_ sai_object_id_t entry_id);\n\n' % table.cname
		enum_txt += create_header(brief='Set attribute for %s_entry' % table.cname, 
								  params_in=['entry_id Entry id', 'attr Attribute'], 
								  return_='#SAI_STATUS_SUCCESS on success Failure status code on error')
		enum_txt += 'typedef sai_status_t(*sai_set_%s_entry_attribute_fn)(\n        _In_ sai_object_id_t entry_id,\n        _In_ const sai_attribute_t *attr);\n\n' % table.cname
		enum_txt += create_header(brief='Get attribute for %s_entry' % table.cname, 
								  params_inout=['attr_list Array of attributes'], 
								  params_in=['entry_id Entry id', 'attr_count Number of attributes'], 
								  return_='#SAI_STATUS_SUCCESS on success Failure status code on error')
		enum_txt += 'typedef sai_status_t(*sai_get_%s_entry_attribute_fn)(\n        _In_ sai_object_id_t entry_id,\n        _In_ uint32_t attr_count,\n        _Inout_ sai_attribute_t *attr_list);\n\n' % table.cname
	# Stats
	enum_txt += create_header(brief='Get statistics counters.', 
							  params_out=['counters Array of resulting counter values.'], 
							  params_in=['entry_id Entry id', 'number_of_counters Number of counters in the array', 'counter_ids Specifies the array of counter ids'], 
							  return_='#SAI_STATUS_SUCCESS on success Failure status code on error')
	enum_txt += 'typedef sai_status_t(*sai_get_%s_stats_fn)(\n        _In_ sai_object_id_t entry_id,\n        _In_ uint32_t number_of_counters,\n        _In_ const sai_%s_stat_t *counter_ids,\n        _Out_ uint64_t *counters);\n\n' % (lib.name, lib.name)
	enum_txt += create_header(brief='Clear statistics counters.', 
							  params_in=['entry_id Entry id', 'number_of_counters Number of counters in the array', 'counter_ids Specifies the array of counter ids'], 
							  return_='#SAI_STATUS_SUCCESS on success Failure status code on error')
	enum_txt += 'typedef sai_status_t(*sai_clear_%s_stats_fn)(\n        _In_ sai_object_id_t entry_id,\n        _In_ uint32_t number_of_counters,\n        _In_ const sai_%s_stat_t *counter_ids);\n\n' % (lib.name, lib.name)
	return enum_txt
def sai_write_object_type(lib, obj_num):
	c_code = ''
	for table in lib.tables:
		c_code += '    SAI_OBJECT_TYPE_%s_ENTRY                = %d,\n' % (table.cname.upper(), obj_num)
		obj_num += 1 
	c_code += '    SAI_OBJECT_TYPE_MAX                      = %d,\n' % obj_num
	return c_code

def sai_write_api_initialize(lib):
	if_list = ''
	for pipe in lib.flexPipes:
		if any(table.flexPipe == pipe for table in lib.tables): # check if pipe is empty
			if_list += 'sai_object_list_t %s_if_list, ' % pipe
	if_list = if_list[:-2]
	c_code = 'sai_status_t sai_ext_api_initialize(%s);\n' % if_list
	c_code += 'sai_status_t sai_ext_api_uninitialize(%s);\n' % if_list
	return c_code

def sai_write_stats_def(lib):
	c_code = create_header(brief = 'Counter IDs in sai_get_%s_stats() call' % (lib.name))
	c_code += 'typedef enum _sai_%s_stat_t\n{\n' % lib.name
	for table in lib.tables:
		c_code += '    SAI_%s_STAT_%s_HIT_PACKETS,\n' % (lib.name.upper(), table.cname.upper())
		c_code += '    SAI_%s_STAT_%s_HIT_OCTETS,\n' % (lib.name.upper(), table.cname.upper())
	c_code+='} sai_%s_stat_t;\n' % lib.name
	return c_code

def sai_create_lib_headers(lib, template_dir):
	# sai|filename|.h
	with open(lib.h_template_path,'r') as t, open (lib.h_path,'w') as o:
		lines = t.readlines()
		for line in lines:
			line = line.replace('|FILENAME|',lib.name.upper())
			line = line.replace('|filename|',lib.name.lower())
			if '__PER_TABLE_ACTION_ENUM__' in line:
				line = sai_write_table_action_enum(lib)
			elif '__PER_TABLE_ATTR_ENUM__' in line:
				line = sai_write_table_attr(lib)
			elif '__STATS_DEF__' in line:
				line = sai_write_stats_def(lib)
			elif '__PER_TABLE_FN_DEF__' in line:
				line = sai_write_table_fn_def(lib)
			elif '__PER_TABLE_API_FN__' in line:
				line = sai_write_table_api_fn(lib)
			elif '__EXT_API_INITIALIZE__' in line:
				line = sai_write_api_initialize(lib)
			o.write(line)
	# sai.h
	with open(os.path.join(template_dir,'sai.h'),'r') as t, open (os.path.join(lib.inc_path, 'sai.h'),'w') as o:
		lines = t.readlines()
		for line in lines:
			line = line.replace('|FILENAME|',lib.name.upper())
			line = line.replace('|filename|',lib.name.lower())
			o.write(line)

	#saitypes.h
	with open(os.path.join(template_dir,'saitypes.h'),'r') as t, open (os.path.join(lib.inc_path, 'saitypes.h'),'w') as o:
		lines = t.readlines()
		for line in lines:
			if '___SAI_OBJECT_TYPE___' in line:
				obj_num = int(line.split('___')[-1])
				line = sai_write_object_type(lib, obj_num)
			o.write(line)
	print('created lib header file')

def sai_create_header(template_path, output_path, lib_name):
	with open(template_path,'r') as t, open (output_path,'w') as o:
		lines = t.readlines()
		for line in lines:
			line = line.replace('|FILENAME|',lib_name.upper())
			line = line.replace('|filename|',lib_name.lower())
			o.write(line)
	print('created ext header file')

def sai_write_object_type_enum(lib):
	enum_txt = ''
	for table in lib.tables:
		enum_txt += '        SAI_OJECT_TYPE_%s_ENTRY,\n' % table.cname.upper()
	return enum_txt

def sai_write_lib_api(lib):
	enum_txt = ''
	for table in lib.tables:
		enum_txt += '        mlnx_create_%s_entry,\n' % table.cname.lower()
		enum_txt += '        mlnx_remove_%s_entry,\n' % table.cname.lower()
		enum_txt += '        mlnx_set_%s_entry_attribute,\n' % table.cname.lower()
		enum_txt += '        mlnx_get_%s_entry_attribute,\n' % table.cname.lower()
	return enum_txt

def sai_write_create_destroy_pipes(lib, cmd):
	enum_txt = ''
	for pipe in lib.flexPipes:
		if any(table.flexPipe == pipe for table in lib.tables): # check if pipe is empty
			enum_txt += '        rc = fx_pipe_%s(fx_handle, FX_%s, (void *)port_list, num_of_ports);\n        if (rc) {\n            printf("Error - rc:%%d\\n", rc);\n            return rc;\n        }\n' % (cmd, pipe.upper())
	return enum_txt

def add_attribute(table_name, attribute_name, attribute_type, attr_key, attr_mask = ''):
	c_code =  '        %s %s_%s;\n' % (attribute_type, table_name, attribute_name)
	if attr_mask != '':
		c_code +=  '        %s %s_%s_mask;\n' % (attribute_type, table_name, attribute_name)
	c_code += '        if (SAI_STATUS_SUCCESS ==\n'
	c_code += '            (sai_status =\n'
	c_code += '                 find_attrib_in_list(attr_count, attr_list, SAI_TABLE_%s_ENTRY_ATTR_%s, &attr, &attr_idx)))\n' % (table_name.upper(), attribute_name.upper())
	c_code += '        {\n'
	if attr_key == 'attr->oid':
		c_code += ("abvd"
				   "asdf"
				  )
	else:
		c_code += '            %s_%s = %s;\n' % (table_name, attribute_name, attr_key)
	if attr_mask != '':
		c_code += '            %s_%s_mask = %s;\n' % (attribute_type, table_name, attribute_name, attr_mask)
	c_code += '        }\n'
	c_code += '        else\n'
	c_code += '        {\n'
	c_code += '            MLNX_SAI_LOG_ERR(\"Did not recieve mandatory %s attribute\\n\");\n' % attribute_name
	c_code += '            return SAI_STATUS_INVALID_PARAMETER;\n'
	c_code += '        }\n'
	return c_code

def get_attr_exact(sai_key_type, sai_key_sdk_type):
	if sai_key_type == 'sai_object_id_t':
		attr_type = sai_key_sdk_type
		attr_key = 'attr->oid'
	if sai_key_type == 'sai_ip_address_t':
		attr_type = 'uint32_t'
		attr_key = 'ntohl((uint32_t) attr->ipaddr.addr.ip4);'
	if sai_key_type == 'sai_uint16_t':
		attr_type = 'uint16_t'
		attr_key = 'attr->u16'
	if sai_key_type == 'sai_uint32_t':
		attr_type = 'uint32_t'
		attr_key = 'attr->u32'
	return attr_type, attr_key

def get_attr_ternary(sai_key_type, sai_key_sdk_type):
	if sai_key_type == 'sai_object_id_t':
		attr_type = sai_key_sdk_type
		attr_key = 'attr->oid'
	if sai_key_type == 'sai_ip_address_t':
		attr_type = 'uint32_t'
		attr_key = 'ntohl((uint32_t) attr->ipaddr.addr.ip4);'
	if sai_key_type == 'sai_uint16_t':
		attr_type = 'uint16_t'
		attr_key = 'attr->u16'
	if sai_key_type == 'sai_uint32_t':
		attr_type = 'uint32_t'
		attr_key = 'attr->u32'
	return attr_type, attr_key, attr_mask

def sai_get_attribute_values(lib, table):
	c_code = ''
	name = table.cname.split('table_')[-1]
	c_code += add_attribute(name, 'action', 'flextrum_action_id_t', 'attr->s32')
	if ('ternary' in table.key_types): # need offset attribute
		c_code += add_attribute(name, 'priority', 'uint32_t', 'attr->u32')
	for key_type, key in zip(table.key_types,table.key_fields):
			sai_key_name, sai_key_type, sai_key_sdk_type, sai_object_type = get_sai_key(lib, key)
			if key_type == 'exact':
				attr_type, attr_key = get_attr_exact(sai_key_type, sai_key_sdk_type)
				c_code += add_attribute(name, sai_key_name, attr_type, attr_key, '')
			# if key_type == 'ternary':
				# c_code += add_attribute(name, sai_key_name, get_attr_value_str(sai_key_type), key_type)
	return c_code

def create_outputs(lib):
	# TODO - take paths in \ out of lib for all paths
	print('\n==================================\nCreating SAI extention header file\n==================================')
	template_dir = os.path.join(lib.backend_path,'output_stage','SAI_templates')
	template_path = os.path.join(template_dir, 'sai_template.h')
	api_set_lib_paths(lib, template_dir)
	sai_create_lib_headers(lib, template_dir)
	sys.exit(0)
