Before SAI version v1.15, *sai_port_oper_status_notification_t* structure was containing 2 members, port_id and port_state.

```C
typedef struct _sai_port_oper_status_notification_t { // v1.14.0
    sai_object_id_t port_id;
    sai_port_oper_status_t port_state;
} sai_port_oper_status_notification_t;
```

In vendor use case it could be used like this, when notificaiton was created:

```C
sai_port_state_change_notification_fn fn = ... ; // pointer obtained from SAI_SWITCH_ATTR_PORT_STATE_CHANGE_NOTIFY

if (fn != NULL)
{
    sai_port_oper_status_notification_t data; // sizeof(data) == 16

    data.port_id = xxx; // oif of affected port
    data.port_state = yyy; // actual port status

    fn(1, &data); // callback
}
```

In SAI version v1.15 breaking change was introduced:

```C
typedef struct _sai_port_oper_status_notification_t { // v1.15.0
    sai_object_id_t port_id;
    sai_port_oper_status_t port_state;
    sai_port_error_status_t port_error_status;
} sai_port_oper_status_notification_t;
```

Which added 3rd field to notification (port_error_status).

Which could be used like this:

```C
sai_port_state_change_notification_fn fn = ... ; // pointer obtained from SAI_SWITCH_ATTR_PORT_STATE_CHANGE_NOTIFY

if (fn != NULL)
{
    sai_port_oper_status_notification_t data; // sizeof(data) == 16

    data.port_id = xxx; // oid of affected port
    data.port_state = yyy; // actual port status
    data.port_error_status = zzz; // new bitmap field

    fn(1, &data); // callback
}
```

Fortunetly that change didn't change sizeof(sai_port_oper_status_notification_t) == 16, which is not impatcing
sai_port_state_change_notification_fn data pointer, which would cause troubles when iterating over notification
data using next indexes, since allignment of structures would be off.

Now this is my proposal, we bring back structure sai_port_oper_status_notification_t to as it was in v1.14
by removing port_error_status field, and introduce attr_count and attr_list fields:

```C
typedef struct _sai_extended_port_oper_status_notification_t
{
    /**
     * @brief Port id.
     *
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_BRIDGE_PORT, SAI_OBJECT_TYPE_LAG
     */
    sai_object_id_t port_id;

    /** Port operational status */
    sai_port_oper_status_t port_state;

    /** Bitmap of various port error or fault status */
    sai_port_error_status_t port_error_status;

    /** Attributes count */
    uint32_t attr_count;

    /**
     * @brief Attributes
     *
     * Object type NULL specifies that attribute list is for object type
     * specified in port_id field. For example if port_id field contains LAG
     * object then list of attributes contains SAI_LAG_ATTR_* attributes.
     *
     * @objects SAI_OBJECT_TYPE_NULL
     */
    sai_attribute_t *attr_list;

} sai_extended_port_oper_status_notification_t;
```

This could be strucutre used from SAI v1.16.0.
Let's use new extended notification type like this:

```C
// still support previous notification

sai_port_state_change_notification_fn fn = ... ; // obtained from SAI_SWITCH_ATTR_PORT_STATE_CHANGE_NOTIFY

if (fn != NULL)
{
    sai_port_oper_status_notification_t data; // sizeof(data) == 16

    data.port_id = xxx; // oif of affected port
    data.port_state = yyy; // actual port status

    fn(1, &data); // callback
}

// here support new notification

sai_extended_port_state_change_notification_fn extfn = ...; // pointer obtained from SAI_SWITCH_ATTR_EXTENDED_PORT_STATE_CHANGE_NOTIFY

if (extfn != NULL)
{
    sai_extended_port_oper_status_notification_t data;

    data.port_id = xxx; // oid of affected port
    data.port_state = yyy; // actual port status
    data.port_error_status = zzz; // bitmap value of port error status

    sai_attribute_t list[1];

    // example attribute with value at the time of the event for port_id fired

    list[0].id = SAI_PORT_ATTR_LINK_TRAINING_RX_STATUS;
    list[0].value.s32 = SAI_PORT_LINK_TRAINING_FAILURE_STATUS_NO_ERROR;

    data.attr_count = 1;
    data.attr_list = list;

    fn(1, &data); // callback for extended notification
}
```

With this solution we are still backward compatible with old notification and we also can support new one.
Advantage is that we can add as many new members to the attr_list as we want not breaking compatibility in the
future.

The only hiccup is that we have 2 breaking changes:
* SAI version v1.14.0 sai_port_oper_status_notification_t have 2 fields (sizeof == 16)
* SAI version v1.15.0 sai_port_oper_status_notification_t have 3 fields (sizeof == 16) - breaking changea - adding port_error_status
* SAI version v1.16.0 sai_port_oper_status_notification_t have 2 fields (sizeof == 16) - breaking change - removing port_error_status

but we will keep compatibility over all previous and future version for this strucute, and all new extensions would be added in new notification.
