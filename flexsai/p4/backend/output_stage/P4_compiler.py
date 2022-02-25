# P4 compiler
# deps:  P4c-bmv2

################################################################
from subprocess import call, Popen, PIPE
import re
import os
import sys
import getopt
import argparse
import json

from shutil import copy2
from glob import glob

################################################################
# args handler
print('PD code generation')
parser = argparse.ArgumentParser(description='Compile P4 program for sai from json')
parser.add_argument('json_path', metavar='JSON', type=str, help='path to json file')
parser.add_argument('-o', '--output_path', help='output dir', nargs='?', default='use_json_path') # TODO remove, p4c support output path, sojson path is output path.
parser.add_argument('-b', '--backend_path', help='backend path')
parser.add_argument('-p', '--p4runtime_config_path', nargs = '?',help='p4runtime json path')
parser.add_argument('--no_cli', dest='create_cli', action='store_const',
                   const=False, default=True,
                   help='create cli (default: True)')
parser.add_argument('--visio', dest='create_visio', action='store_const',
                   const=True, default=False,
                   help='create visio output (default: False)')
parser.add_argument('-a', '--api',default='SAI', help='possible API types - SDK, SAI, P4runtime')
parser.add_argument('-v', '--verbose',action = 'store_true')

args = parser.parse_args()
if args.output_path == 'use_json_path':
	args.output_path = os.path.split(args.json_path)[0] # removes '.json' from json path
elif os.path.isabs(args.output_path)==False:
	args.output_path = os.path.join(
		os.getcwd(),
		args.output_path
		)

print('argv: ')
print(sys.argv)
print('args:')
print(args)
################################################################
# local imports
sys.path.append(os.path.join(args.backend_path,'output_stage'))
import P4_aux as aux
# import P4_base_layer as base

####################################################################
class FlexLib(object):
	def __init__(self, name, backend_path, output_path, json_path, runtime_config_path):
		# set variables
		self.name = name
		# TODO: self.app_name = app_name
		self.output_path = output_path
		self.backend_path = backend_path
		self.runtime_config_path = runtime_config_path
		self.json_path = json_path
		self.flexPipes = ['in_port','in_rif','out_rif','out_port']
		
		# Create output dirs:
		aux.ensure_dir(output_path)
		for d in ['src','cli','lib','inc','visio','sai_inc', 'sai_src']:
			aux.ensure_dir(os.path.join(output_path,d))
		
		# copy includes	
		dst_inc = os.path.join(backend_path,'output_stage','inc')
		for filename in glob(os.path.join(dst_inc,'*.*')):
			copy2(filename, os.path.join(output_path,'inc'))

		# get sai data
		self.pipe_table_count ={}
		self.tables = []
		self.p4_action_def = []
		self.app_action_list = []
		try: 
			keys_path = os.path.join(backend_path,'output_stage','json','sai_keys.json')
			actions_path = os.path.join(backend_path,'output_stage','json','sai_actions.json')
			# fixed_headers_path = os.path.join(backend_path,'output_stage','json','sai_fixed_headers.json')
			metadata_path = os.path.join(backend_path,'output_stage','json','sai_metadata.json')
			self.sai_keys = json.load(open(keys_path,'r'))
			self.sai_actions = json.load(open(actions_path,'r'))
			# self.sai_fixed_headers = json.load(open(fixed_headers_path,'r'))
			self.sai_metadata = json.load(open(metadata_path,'r'))
			self.runtime_config = json.load(open(runtime_config_path, 'r'))
		except IOError as e:
			print("ERROR ({0}): load json action/key db failed. {1}. exiting.".format(e.errno,e.strerror))
			sys.exit(1)

	def add_table_to_lib(self, FlexTable):
		self.tables.append(FlexTable)

	def create_visio(self): # support up to two tables per flow:
		out_dir = os.path.join(self.output_path,'visio')
		aux.ensure_dir(out_dir)
		
		bm_tables_dict = {};
		bm_tables_dict[('in_port',0)] = '		<g id="shape196-1427" v:mID="196" v:groupContext="shape" transform="translate(566.929,-864.567)">\n			<title>Sheet.196</title>\n			<desc>in_port_1</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)"/>\n			<v:textRect cx="63.7795" cy="1657.7" width="127.56" height="212.598"/>\n		<rect x="0" y="1551.4" width="127.559" height="212.598" class="st41"/>\n			<text x="38.76" y="1570.3" class="st37" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>\n'
		bm_tables_dict[('in_port',1)] = '		<g id="shape197-1430" v:mID="197" v:groupContext="shape" transform="translate(744.094,-864.567)">\n			<title>Sheet.197</title>\n			<desc>in_port_2</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)"/>\n			<v:textRect cx="63.7795" cy="1657.7" width="127.56" height="212.598"/>\n			<rect x="0" y="1551.4" width="127.559" height="212.598" class="st41"/>\n			<text x="38.76" y="1570.3" class="st37" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>\n'
		bm_tables_dict[('in_rif',0)]  = '		<g id="shape202-1433" v:mID="202" v:groupContext="shape" transform="translate(708.661,-248.031)">\n			<title>Sheet.202</title>\n			<desc>in_rif_1</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)"/>\n			<v:textRect cx="63.7795" cy="1657.7" width="127.56" height="212.598"/>\n			<rect x="0" y="1551.4" width="127.559" height="212.598" class="st41"/>\n			<text x="44.1" y="1570.3" class="st37" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>\n'
		bm_tables_dict[('in_rif',1)]  = '		<g id="shape203-1436" v:mID="203" v:groupContext="shape" transform="translate(921.26,-248.031)">\n			<title>Sheet.203</title>\n			<desc>in_rif_2</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)"/>\n			<v:textRect cx="63.7795" cy="1657.7" width="127.56" height="212.598"/>\n			<rect x="0" y="1551.4" width="127.559" height="212.598" class="st41"/>\n			<text x="44.1" y="1570.3" class="st37" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>\n'
		bm_tables_dict[('out_rif',0)] = '		<g id="shape204-1439" v:mID="204" v:groupContext="shape" transform="translate(2444.88,-248.031)">\n			<title>Sheet.204</title>\n			<desc>out_rif_1</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)"/>\n			<v:textRect cx="63.7795" cy="1657.7" width="127.56" height="212.598"/>\n			<rect x="0" y="1551.4" width="127.559" height="212.598" class="st41"/>\n			<text x="40.43" y="1570.3" class="st37" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>\n'
		bm_tables_dict[('out_rif',1)] = '		<g id="shape205-1442" v:mID="205" v:groupContext="shape" transform="translate(2636.22,-248.031)">\n			<title>Sheet.205</title>\n			<desc>out_rif_2</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)"/>\n			<v:textRect cx="63.7795" cy="1657.7" width="127.56" height="212.598"/>\n			<rect x="0" y="1551.4" width="127.559" height="212.598" class="st41"/>\n			<text x="40.43" y="1570.3" class="st37" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>\n'
		
		sai_tables_dict={}
		sai_tables_dict[('in_port',0)]  = '		<g id="shape20-104" v:mID="20" v:groupContext="shape" transform="translate(152.953,-320.315)">\n			<title>Sheet.20</title>\n			<desc>__in_p_1__</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)" v:tabSpace="42.5197"/>\n			<v:textRect cx="49.6063" cy="535.748" width="99.22" height="119.055"/>\n			<g id="shadow20-105" v:groupContext="shadow" v:shadowOffsetX="0.345598" v:shadowOffsetY="-1.97279" v:shadowType="1"\n					transform="matrix(1,0,0,1,0.345598,1.97279)" class="st1">\n				<rect x="0" y="476.22" width="99.2126" height="119.055" class="st2"/>\n			</g>\n			<rect x="0" y="476.22" width="99.2126" height="119.055" class="st12"/>\n			<text x="20.95" y="490.35" class="st4" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>'
		sai_tables_dict[('in_port',1)]  = '		<g id="shape21-110" v:mID="21" v:groupContext="shape" transform="translate(286.417,-320.315)">\n			<title>Sheet.21</title>\n			<desc>__in_p_2__</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)" v:tabSpace="42.5197"/>\n			<v:textRect cx="49.6063" cy="535.748" width="99.22" height="119.055"/>\n			<g id="shadow21-111" v:groupContext="shadow" v:shadowOffsetX="0.345598" v:shadowOffsetY="-1.97279" v:shadowType="1"\n					transform="matrix(1,0,0,1,0.345598,1.97279)" class="st1">\n				<rect x="0" y="476.22" width="99.2126" height="119.055" class="st2"/>\n			</g>\n			<rect x="0" y="476.22" width="99.2126" height="119.055" class="st12"/>\n			<text x="20.95" y="490.35" class="st4" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>'
		sai_tables_dict[('in_rif',0)]   = '		<g id="shape23-116" v:mID="23" v:groupContext="shape" transform="translate(552.756,-320.315)">\n			<title>Sheet.23</title>\n			<desc>__in_r_1__</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)" v:tabSpace="42.5197"/>\n			<v:textRect cx="49.6063" cy="535.748" width="99.22" height="119.055"/>\n			<g id="shadow23-117" v:groupContext="shadow" v:shadowOffsetX="0.345598" v:shadowOffsetY="-1.97279" v:shadowType="1"\n					transform="matrix(1,0,0,1,0.345598,1.97279)" class="st1">\n				<rect x="0" y="476.22" width="99.2126" height="119.055" class="st2"/>\n			</g>\n			<rect x="0" y="476.22" width="99.2126" height="119.055" class="st12"/>\n			<text x="22.01" y="490.35" class="st4" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>'
		sai_tables_dict[('in_rif',1)]   = '		<g id="shape24-122" v:mID="24" v:groupContext="shape" transform="translate(686.22,-320.315)">\n			<title>Sheet.24</title>\n			<desc>__in_r_2__</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)" v:tabSpace="42.5197"/>\n			<v:textRect cx="49.6063" cy="535.748" width="99.22" height="119.055"/>\n			<g id="shadow24-123" v:groupContext="shadow" v:shadowOffsetX="0.345598" v:shadowOffsetY="-1.97279" v:shadowType="1"\n					transform="matrix(1,0,0,1,0.345598,1.97279)" class="st1">\n				<rect x="0" y="476.22" width="99.2126" height="119.055" class="st2"/>\n			</g>\n			<rect x="0" y="476.22" width="99.2126" height="119.055" class="st12"/>\n			<text x="22.01" y="490.35" class="st4" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>'
		sai_tables_dict[('out_rif',0)]  = '		<g id="shape25-128" v:mID="25" v:groupContext="shape" transform="translate(552.756,-51.0236)">\n			<title>Sheet.25</title>\n			<desc>__out_r_2__</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)" v:tabSpace="42.5197"/>\n			<v:textRect cx="49.6063" cy="535.748" width="99.22" height="119.055"/>\n			<g id="shadow25-129" v:groupContext="shadow" v:shadowOffsetX="0.345598" v:shadowOffsetY="-1.97279" v:shadowType="1"\n					transform="matrix(1,0,0,1,0.345598,1.97279)" class="st1">\n				<rect x="0" y="476.22" width="99.2126" height="119.055" class="st2"/>\n			</g>\n			<rect x="0" y="476.22" width="99.2126" height="119.055" class="st12"/>\n			<text x="18.22" y="490.35" class="st4" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>'
		sai_tables_dict[('out_rif',1)]  = '		<g id="shape26-134" v:mID="26" v:groupContext="shape" transform="translate(686.22,-51.0236)">\n			<title>Sheet.26</title>\n			<desc>__out_r_1__</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)" v:tabSpace="42.5197"/>\n			<v:textRect cx="49.6063" cy="535.748" width="99.22" height="119.055"/>\n			<g id="shadow26-135" v:groupContext="shadow" v:shadowOffsetX="0.345598" v:shadowOffsetY="-1.97279" v:shadowType="1"\n					transform="matrix(1,0,0,1,0.345598,1.97279)" class="st1">\n				<rect x="0" y="476.22" width="99.2126" height="119.055" class="st2"/>\n			</g>\n			<rect x="0" y="476.22" width="99.2126" height="119.055" class="st12"/>\n			<text x="18.22" y="490.35" class="st4" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>'
		sai_tables_dict[('out_port',0)] = '		<g id="shape27-140" v:mID="27" v:groupContext="shape" transform="translate(150,-51.0236)">\n			<title>Sheet.27</title>\n			<desc>__out_p_2__</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)" v:tabSpace="42.5197"/>\n			<v:textRect cx="49.6063" cy="535.748" width="99.22" height="119.055"/>\n			<g id="shadow27-141" v:groupContext="shadow" v:shadowOffsetX="0.345598" v:shadowOffsetY="-1.97279" v:shadowType="1"\n					transform="matrix(1,0,0,1,0.345598,1.97279)" class="st1">\n				<rect x="0" y="476.22" width="99.2126" height="119.055" class="st2"/>\n			</g>\n			<rect x="0" y="476.22" width="99.2126" height="119.055" class="st12"/>\n			<text x="17.16" y="490.35" class="st4" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>'
		sai_tables_dict[('out_port',1)] = '		<g id="shape28-146" v:mID="28" v:groupContext="shape" transform="translate(283.465,-51.0236)">\n			<title>Sheet.28</title>\n			<desc>__out_p_1__</desc>\n			<v:textBlock v:margins="rect(4,4,4,4)" v:tabSpace="42.5197"/>\n			<v:textRect cx="49.6063" cy="535.748" width="99.22" height="119.055"/>\n			<g id="shadow28-147" v:groupContext="shadow" v:shadowOffsetX="0.345598" v:shadowOffsetY="-1.97279" v:shadowType="1"\n					transform="matrix(1,0,0,1,0.345598,1.97279)" class="st1">\n				<rect x="0" y="476.22" width="99.2126" height="119.055" class="st2"/>\n			</g>\n			<rect x="0" y="476.22" width="99.2126" height="119.055" class="st12"/>\n			<text x="17.16" y="490.35" class="st4" v:langID="1033"><v:paragraph v:horizAlign="1"/><v:tabList/>__TEXT__</text>		</g>'
		
		visio_dicts = [sai_tables_dict,bm_tables_dict]

		endl = '</tspan><tspan x="3" dy="1.2em" class="st299">'
		visios = ['sai_target.svg','flex_bm.svg']
		for i,visio in enumerate(visios):
			pipe_index ={}
			pipe_index['in_port'] = 0
			pipe_index['in_rif'] = 0
			pipe_index['out_rif'] = 0
			pipe_index['out_port'] = 0
			with open(os.path.join(self.backend_path,'output_stage','visio','template',visio),'r') as t, open(os.path.join(out_dir,'visio')+visio,'w') as o:
				for line in t.readlines():
					if '__ADD_FLEX_TABLES__' in line:
						for table in self.tables:
							table_text = '<tspan x="10">' + table.name +  endl +'match: {'+endl
							table_text+= endl.join([x +':'+ y +' ' for x,y in zip(table.key_fields_json,table.key_types)])
							table_text+= endl+ '}'+endl+' actions: {'+endl
							table_text+= endl.join([a for a in table.actions]) +endl+ '}</tspan>' # TODO
							#table_text = fix_lines(table_text)
							new_line =''
							try:
								new_line = visio_dicts[i][(table.flexPipe,pipe_index[table.flexPipe])].replace('__TEXT__',table_text)
							except KeyError:
								print('Warning: [visio: %s] table skipped, no place in template'%visio)
								pass
							o.write(new_line)
							pipe_index[table.flexPipe] +=1

					else:
						o.write(line)
	
	def get_header_key(self,key0,key1):
		if args.verbose : print ('getting key: %s.%s'%(key0,key1))
		# if key0 in self.sai_fixed_headers:
			# inner = self.sai_fixed_headers[key0]
			# if key1 in inner:
				# return inner[key1] 
		if key0 in self.sai_metadata:
			inner = self.sai_metadata[key0]
			if key1 in inner:
				return inner[key1] 
		print ('Error - unsupported table key! %s. %s'%(key0,key1) )
		sys.exit(1)
	
	# get the appropriate ACL extraction point	
	def get_extraction_point(self,header):
		if args.verbose : print ('getting extraction point: %s' %header)
		if header in self.sai_fixed_extractions:
			return self.sai_fixed_extractions[header] 
		if header in self.sai_flex_extractions:
			return self.sai_flex_extractions[header] 
		print ('Error - unsupported extraction point! %s' %header )
		sys.exit(1)
	
		# get the runtime table ID from the p4info JSON
	def get_table_id(self, table_name):
		for table in self.runtime_config['tables']:
			if table['preamble']['name'] == table_name:
				return table['preamble']['id']
		print ('Error - unsupported table: %s' %table_name )
		sys.exit(1)
	
	# get the runtime action ID from the p4info JSON
	def get_action_id(self, action_name):
		for action in self.runtime_config['actions']:
			name = action['preamble']['name']
			if name.upper() == action_name:
				return action['preamble']['id']
		print ('Error - unsupported action: %s' %action_name )
		sys.exit(1) 	
	
	# get the action index from the P4 program JSON output	
	def get_action_index(self, action_name):
		for action in self.p4_action_def:
			name = action['name']
			if name.upper() == action_name:
				return action['id']
		print ('Error - unsupported action: %s' %action_name )
		sys.exit(1) 		
		

	def get_action_cname(self, action_name):
		return aux.get_canonical_c_name(action_name).upper()
####################################################################
class FlexTable(object):
	def __init__(self,flexPipe,name,id,size,key_types,key_fields,key_fields_json,masks,actions,action_ids,key_params,cb_key_index):
		self.flexPipe = flexPipe
		self.name = name
		self.cname = aux.get_canonical_c_name(name)
		self.id = id
		self.size = size
		self.key_types = key_types
		self.key_fields = key_fields
		self.key_fields_json = key_fields_json # for visio only
		self.masks = masks
		self.actions = actions
		self.cactions = []
		for action in actions:
			self.cactions.append(aux.get_canonical_c_name(action).upper())
		self.action_ids = action_ids		
		self.key_params = key_params
		self.cb_key_index = cb_key_index

	def get_key_field_count(self):
		i = 0
		for field_list in self.key_fields:
			for field in field_list:
				i=i+1
		return i

	def isAllExact(self):
		return all(key_type == 'exact' for key_type in self.key_types)

	def printTable(self):
		print('-------- \n Table %s \n--------'%self.name)
		print('control pipe: %s'%self.flexPipe)
		print('size: %s'%self.size)
		print('key_types: %s'%self.key_types)
		print('key_fields: %s'%self.key_fields)
		print('key_params: %s'%(self.key_params))
		print('masks: %s'%self.masks)
		print('actions: %s'%self.actions)
		print('------------------------\n\n')


####################################################################
def interp_p4c_json_stage_output(lib,json_path):
	# bmv2 json doesn't save tables in order, need to search by 'base_default_next' field

	print('interp ' + json_path)
	with open(json_path,'r') as json_file:
		j = json.load(json_file)
		for pipe in lib.flexPipes:
			# TODO (readability) make dict prio to prevent the below loop
			for jpipenum,jpipe in enumerate(j['pipelines']):
				if jpipe['name'] == 'control_'+pipe:
					if args.verbose:
						print ('===============================================================')
						print ('Pipeline : control_' + pipe )
						print ('===============================================================')
					break
			lib.p4_action_def = j["actions"]
			next_table_name = j['pipelines'][jpipenum]['init_table']
			if args.verbose : print('>>>> 1st table:'+ next_table_name if next_table_name != None else ' null')
			inserted_tables = 0 
			tables = j['pipelines'][jpipenum]['tables']
			while inserted_tables < len(tables) and next_table_name != None:
				for table in tables:
					table_name = table['name'] 
					if args.verbose : print("pipe %s. table %s" % (pipe, table_name))
					if table_name != next_table_name and next_table_name != None:
						continue
					table_size = table['max_size']
					actions = table['actions']
					action_ids = table['action_ids']
					key_types = []
					key_fields = []
					key_fields_json = []
					key_masks = []
					key_params = []
					custom_key_offset = []
					#print j['pipelines'][0]['tables'][table_num]['key']
					for i,key in enumerate(table['key']):
						key_types.append(key['match_type'])
						target_header = key['target'][0]
						target_field = key['target'][1]
						json_target = target_header + '.' + target_field
						key_fields_json.append(json_target)
						key_fields.append(lib.get_header_key(target_header,target_field))
						key_masks.append(key['mask'])
						# target consists of a header and a field
						# see if the target header is a predefine one
						is_pre_extracted = is_field_pre_extracted(j,target_header,target_field)

						#get the custom bytes info
						# if is_pre_extracted != True:
							# key_params.append(key['mlnx_extract']);
							# custom_key_offset.append(i)
							# if args.verbose : print 'getting custom key ' + json_target
						if args.verbose : print('getting predefined key ' + json_target)
											
					#print key_params
					if (len(custom_key_offset) > 1):
						print ('Error - more than 1 custom byte set per table key is not supported (%s)'%str(key_fields) )
						sys.exit(1)
					table_id = lib.get_table_id(table_name)
					flextable = FlexTable(pipe,table_name,table_id,table_size,key_types,key_fields,key_fields_json,key_masks,actions,action_ids,key_params,custom_key_offset)
					if args.verbose : flextable.printTable()
					lib.add_table_to_lib(flextable)
					inserted_tables+=1
					#TODO- process static entries
					if args.verbose  and 'entries' in table:
						for ent in table['entries']:
							print(ent['match_key'])
							# add to table
					next_table_name = table['base_default_next']
			if args.verbose  : print ('inserted tables: %d'%inserted_tables)
			lib.pipe_table_count[pipe]=inserted_tables		
	return

## In the JSON output, find the headers and see if it is a hard coded header
def is_header_predefined(json_obj, header):
	for ph in json_obj['headers']:
		if ph['name'] == header and 'mlnx_predefined' in ph:
			return ph['mlnx_predefined']
	
## in the JSON output, find the header_types and see if the field is automatically extracted	
## a pre-extracted field can only come from a predefined header	
def is_field_pre_extracted(json_obj, header, field_name):
	for ph in json_obj['headers']:
		if ph['name'] == header and 'mlnx_predefined' in ph:
			header_type = ph['header_type']
			for ht in json_obj['header_types']:
				if ht['name'] == header_type:
					for fldlist in ht['fields']:
						for fld in fldlist:
							if fld == field_name:
								# Header field format is [NAME,  size, is_signed, is_extracted]
								if len(fldlist) == 4: 
									return fldlist[3]
								else:
									print('Error parsing header field: ' + fld)
									print(fldlist)
									exit(0)
					break
			break
	return False

################################################################
# Main flow
# create flex-lib C code for adding and removing table entries (using mlnx sdk)
lib_name = 'lib'+args.json_path.split('/')[-1].split('.')[0] # Todo - add as an argument?
lib = FlexLib(lib_name, args.backend_path, args.output_path, args.json_path, os.path.abspath(args.p4runtime_config_path))
# try:
print('\n==================================\nInterperting JSON file\n==================================')
interp_p4c_json_stage_output(lib,args.json_path)
print('\n==================================\nCreating Base layer\n==================================')
# base.base_set_path(lib)
# base.layer_create(lib)
if args.create_visio:
	lib.create_visio()
# if args.create_cli:
	# create_cli()
print('\n==================================\nCreating %s API \n=================================='%args.api)
if args.api == 'P4runtime':
	# import P4_api_grta as api
	print ("This version doesn't support P4rt api")
	exit(1)
if args.api == 'SDK': 
	# import P4_api_SDK as api
	print ("This version doesn't support SDK api")
	exit(1)
if args.api == 'SAI':
	import P4_api_SAI as api
# api.API_set_path(lib)
api.create_outputs(lib)

# if args.create_cli and args.api == 'SDK' : 
# 	print('\n==================================\nCreating CLI APP \n==================================')
# 	from P4_app_CLI import *
# 	cli_set_path(lib)
# 	cli_create_outputs(lib)



