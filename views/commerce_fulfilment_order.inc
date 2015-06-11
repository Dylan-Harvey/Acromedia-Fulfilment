<?php

$view = new view();
$view->name = 'commerce_fulfilment_order';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'commerce_order_revision';
$view->human_name = 'Commerce Fulfilment Order';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'commerce_fulfilment_order';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '5';
$handler->display->display_options['style_plugin'] = 'table';
/* Field: Commerce Order revision: Order number */
$handler->display->display_options['fields']['order_number']['id'] = 'order_number';
$handler->display->display_options['fields']['order_number']['table'] = 'commerce_order_revision';
$handler->display->display_options['fields']['order_number']['field'] = 'order_number';
/* Field: Commerce Order (historical data): Line items */
$handler->display->display_options['fields']['commerce_line_items-revision_id']['id'] = 'commerce_line_items-revision_id';
$handler->display->display_options['fields']['commerce_line_items-revision_id']['table'] = 'field_revision_commerce_line_items';
$handler->display->display_options['fields']['commerce_line_items-revision_id']['field'] = 'commerce_line_items-revision_id';
$handler->display->display_options['fields']['commerce_line_items-revision_id']['settings'] = array(
  'view' => 'commerce_line_item_table|default',
);
$handler->display->display_options['fields']['commerce_line_items-revision_id']['delta_offset'] = '0';
/* Field: Commerce Order revision: E-mail */
$handler->display->display_options['fields']['mail']['id'] = 'mail';
$handler->display->display_options['fields']['mail']['table'] = 'commerce_order_revision';
$handler->display->display_options['fields']['mail']['field'] = 'mail';
/* Contextual filter: Commerce Order revision: Order number */
$handler->display->display_options['arguments']['order_number']['id'] = 'order_number';
$handler->display->display_options['arguments']['order_number']['table'] = 'commerce_order_revision';
$handler->display->display_options['arguments']['order_number']['field'] = 'order_number';
$handler->display->display_options['arguments']['order_number']['default_action'] = 'empty';
$handler->display->display_options['arguments']['order_number']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['order_number']['default_argument_options']['argument'] = 'all';
$handler->display->display_options['arguments']['order_number']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['order_number']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['order_number']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['order_number']['limit'] = '0';
/* Filter criterion: Commerce Order revision: Order status */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'commerce_order_revision';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = array(
  'checkout_review' => 'checkout_review',
);
$handler->display->display_options['filters']['status']['group'] = 1;

/* Display: Order Block */
$handler = $view->new_display('block', 'Order Block', 'order_block');
$handler->display->display_options['block_description'] = 'Order_Block';
$translatables['commerce_fulfilment_order'] = array(
  t('Master'),
  t('commerce_fulfilment_order'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Order number'),
  t('Line items'),
  t('E-mail'),
  t('All'),
  t('Order Block'),
  t('Order_Block'),
);
