<?php
$view = new view();
$view->name = 'commerce_fulfillment_shipments';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'commerce_fulfillment_shipments';
$view->human_name = 'Commerce Fulfillment Shipments';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Commerce Fulfillment Shipments';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'table_megarows';
$handler->display->display_options['row_plugin'] = 'entity';
/* Field: Bulk operations: Commerce Fulfillment Shipment */
$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'commerce_fulfillment_shipments';
$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
$handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
'action::commerce_fulfillment_add_action' => array(
'selected' => 0,
'skip_confirmation' => 0,
'override_label' => 0,
'label' => '',
),
'action::views_bulk_operations_delete_item' => array(
'selected' => 1,
'postpone_processing' => 0,
'skip_confirmation' => 0,
'override_label' => 1,
'label' => 'Delete Shipment',
),
'action::views_bulk_operations_delete_revision' => array(
'selected' => 0,
'postpone_processing' => 0,
'skip_confirmation' => 0,
'override_label' => 0,
'label' => '',
),
'action::views_bulk_operations_script_action' => array(
'selected' => 0,
'postpone_processing' => 0,
'skip_confirmation' => 0,
'override_label' => 0,
'label' => '',
),
'action::views_bulk_operations_modify_action' => array(
'selected' => 0,
'postpone_processing' => 0,
'skip_confirmation' => 0,
'override_label' => 0,
'label' => '',
'settings' => array(
'show_all_tokens' => 1,
'display_values' => array(
'_all_' => '_all_',
),
),
),
'action::views_bulk_operations_argument_selector_action' => array(
'selected' => 1,
'skip_confirmation' => 1,
'override_label' => 1,
'label' => 'Print Shipping Label',
'settings' => array(
'url' => 'shipping_label',
),
),
'action::commerce_fulfillment_remove_action' => array(
'selected' => 0,
'skip_confirmation' => 0,
'override_label' => 0,
'label' => '',
),
'action::system_send_email_action' => array(
'selected' => 0,
'postpone_processing' => 0,
'skip_confirmation' => 0,
'override_label' => 0,
'label' => '',
),
);
/* Field: Commerce Fulfillment Shipment: Commerce fulfillment shipment ID */
$handler->display->display_options['fields']['shipment_id']['id'] = 'shipment_id';
$handler->display->display_options['fields']['shipment_id']['table'] = 'commerce_fulfillment_shipments';
$handler->display->display_options['fields']['shipment_id']['field'] = 'shipment_id';
/* Field: Commerce Fulfillment Shipment: Status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'commerce_fulfillment_shipments';
$handler->display->display_options['fields']['status']['field'] = 'status';
/* Field: Commerce Fulfillment Shipment: Packages */
$handler->display->display_options['fields']['commerce_fulfillment_packages']['id'] = 'commerce_fulfillment_packages';
$handler->display->display_options['fields']['commerce_fulfillment_packages']['table'] = 'field_data_commerce_fulfillment_packages';
$handler->display->display_options['fields']['commerce_fulfillment_packages']['field'] = 'commerce_fulfillment_packages';
$handler->display->display_options['fields']['commerce_fulfillment_packages']['element_type'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_packages']['element_wrapper_type'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_packages']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['commerce_fulfillment_packages']['type'] = 'entityreference_entity_id';
$handler->display->display_options['fields']['commerce_fulfillment_packages']['settings'] = array(
'link' => 0,
);
$handler->display->display_options['fields']['commerce_fulfillment_packages']['delta_offset'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_packages']['field_api_classes'] = TRUE;
/* Field: Commerce Fulfillment Shipment: Tracking_number */
$handler->display->display_options['fields']['tracking_number']['id'] = 'tracking_number';
$handler->display->display_options['fields']['tracking_number']['table'] = 'commerce_fulfillment_shipments';
$handler->display->display_options['fields']['tracking_number']['field'] = 'tracking_number';
/* Field: Field: Shipping Info */
$handler->display->display_options['fields']['commerce_fulfillment_shipping']['id'] = 'commerce_fulfillment_shipping';
$handler->display->display_options['fields']['commerce_fulfillment_shipping']['table'] = 'field_data_commerce_fulfillment_shipping';
$handler->display->display_options['fields']['commerce_fulfillment_shipping']['field'] = 'commerce_fulfillment_shipping';
$handler->display->display_options['fields']['commerce_fulfillment_shipping']['label'] = '';
$handler->display->display_options['fields']['commerce_fulfillment_shipping']['element_type'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_shipping']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['commerce_fulfillment_shipping']['element_wrapper_type'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_shipping']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['commerce_fulfillment_shipping']['delta_offset'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_shipping']['field_api_classes'] = TRUE;
/* Contextual filter: Commerce Fulfillment Shipment: Order_id */
$handler->display->display_options['arguments']['order_id']['id'] = 'order_id';
$handler->display->display_options['arguments']['order_id']['table'] = 'commerce_fulfillment_shipments';
$handler->display->display_options['arguments']['order_id']['field'] = 'order_id';
$handler->display->display_options['arguments']['order_id']['default_action'] = 'not found';
$handler->display->display_options['arguments']['order_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['order_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['order_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['order_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['order_id']['limit'] = '0';

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block');
