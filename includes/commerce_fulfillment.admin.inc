<?php

/**
 * @param $form
 * @param $form_state
 * @return array
 * Implements hook_form
 * Creates an admin page form
 * Will use this data to print packing slips and shipping labels
 */
function commerce_fulfillment_admin_form($form, &$form_state){

  $form = array();

  $form['commerce_fulfillment_title'] = array(
    '#markup' => '<h2>' . t('Admin Page') . '</h2>'
  );

  //Creates separate information fields
  $form['commerce_fulfillment_company_information']=array(
    '#type' => 'fieldset',
    '#title' => t("Enter your company's information below"),
    '#description' => 'Enter information to use on Packing Slips and Shipping Labels.',
    '#prefix' => '<div id="boxtype-field-wrapper">',
    '#suffix' => '</div>',
  );

  //Textfields
  $form['commerce_fulfillment_company_information']['commerce_fulfillment_company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company name:'),
    '#default_value' => variable_get('commerce_fulfillment_company', NULL),
    '#required' => TRUE,
  );
  $form['commerce_fulfillment_company_information']['commerce_fulfillment_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address:'),
    '#default_value' => variable_get('commerce_fulfillment_address', NULL),
    '#required' => TRUE,
  );
  $form['commerce_fulfillment_company_information']['commerce_fulfillment_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Company Phone Number:'),
    '#default_value' => variable_get('commerce_fulfillment_phone', NULL),
    '#required' => TRUE,
  );

  if(!isset($form_state['num_boxtypes'])){
    $form_state['num_boxtypes'] = 1;
  }

  for($i = 0; $i < $form_state['num_boxtypes']; $i++){
    $form['commerce_fulfillment_company_information']['commerce_fulfillment_boxtype'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('Add Package Type:'),
    );
  }

  $form['commerce_fulfillment_company_information']['add_boxtype'] = array(
    '#type' => 'submit',
    '#value' => t('Add Another Package Type'),
    '#submit' => array('commerce_fulfillment_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'commerce_fulfillment_add_more_callback',
      'wrapper' => 'boxtype-field-wrapper',
      'method' => 'replace',
    ),
  );

  //Allows user to upload company logo
  $form['#attributes']['enctype'] = 'multipart/form-data';

  $form['commerce_fulfillment_company_information']['commerce_fulfillment_logo']=array(
    '#type' => 'managed_file',
    '#title' => t('Upload your company logo:'),
    '#default_value' => variable_get('commerce_fulfillment_logo', NULL),
    '#upload_location' => 'public://logo/'
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'commerce_fulfillment_admin_form_submit';

  return $form;
}

/**
 *  @param $form
 *  @param $form_state
 *  Implements hook hook_form_submit
 *  Does not override system_settings_form submit.
 *  Just handles submission of the package_type variable
 */

function commerce_fulfillment_admin_form_submit($form, &$form_state){

  //gets variable package_type
  $package_type = variable_get('commerce_fulfillment_package_type', NULL);

  if($package_type == NULL) {
    $package_type = array();
  }

  //appends global variable package_type
  for($i = $form_state['num_boxtypes']; $i>0; $i--) {
    $package_type[] = $form_state['values'][$i-1];
  }

  variable_set('commerce_fulfillment_package_type', $package_type);
}

/**
 * @param $form
 * @param $form_state
 * @return mixed
 *
 * Ajax Callback for boxtypes
 *
 * Returns the fieldset for the form.
 */
function commerce_fulfillment_add_more_callback($form, &$form_state){
  return $form['commerce_fulfillment_company_information'];
}

/**
 * @param $form
 * @param $form_state
 *
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function commerce_fulfillment_add_more_add_one($form, &$form_state) {
  $form_state['num_boxtypes']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * @param $order
 * @return string
 *
 * Builds the package page.
 * Pulls in Order info and displays the name of the products in chosen Order.
 *
 */
function commerce_fulfillment_package_content($order){

  $order_wrapper = entity_metadata_wrapper( 'commerce_order', $order);
  $order_id = $order_wrapper->order_id->value();

  $get_form = drupal_get_form('commerce_fulfillment_create_package_form', $order_id);
  $output = drupal_render($get_form);

  //@TODO - this has a lot of formatting, consider moving it out into a tpl in the future
  $output .= '<h2>' . t('Products in Order @order_id:', array('@order_id' => $order_id)) . '</h2>';
  $output .= views_embed_view('commerce_fulfillment_line_items', 'line_item_block', $order_id);

  $output .= '<h2>' . t('Packages:') . '</h2>';
  $output .= views_embed_view('commerce_fulfillment_packages', 'package_package_block', $order_id);

  return $output;
}

/**
 * @param $form
 * @param $form_state
 * @param $order_id
 * @return array
 *
 * implements hook_form
 * adds functionality to create a package
 *
 */
function commerce_fulfillment_create_package_form($form, &$form_state, $order_id){

  $package_type = variable_get('commerce_fulfillment_package_type', array(0 => 'No Package Type set.'));

  $form = array();

  $form['commerce_fulfillment_order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order_id,
  );

  $form['commerce_fulfillment_title'] = array(
    '#markup'=>'<h2>Create a New Package:</h2>'
  );

  $form['commerce_fulfillment_package_type'] = array(
    '#type' => 'select',
    '#title' => t('Select package type: '),
    '#options' => $package_type,
    '#default_value' => 'default',
  );

  $form['commerce_fulfillment_create'] = array(
    '#type' => 'submit',
    '#value' => t('Create a Package')
  );

  return $form;
}

/**
 * @param $form
 * @param $form_state
 *
 * implements hook_form_submit
 */
function commerce_fulfillment_create_package_form_submit($form, &$form_state){
  $package_type = variable_get('commerce_fulfillment_package_type', array(0 => 'No Package Type set.'));

  //create an order entity wrapper
  $order_id = $form_state['values']['commerce_fulfillment_order_id'];
  $order = commerce_order_load($order_id);
  $order_wrapper= entity_metadata_wrapper('commerce_order', $order);

  //create a profile entity wrapper
  $profile_id = $order_wrapper->commerce_customer_billing->profile_id->value();
  $profile = commerce_customer_profile_load($profile_id);
  $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);

  //create the package entity
  $entity = entity_create('commerce_fulfillment_package', array(
    'order_id' => $order_wrapper->order_id->value(),
    'box_type' => $package_type[$form_state['values']['commerce_fulfillment_package_type']],
    'commerce_fulfillment_line_items' => NULL,
    'commerce_fulfillment_shipping'=> array(
      'first_name' => $profile_wrapper->commerce_customer_address->first_name->value(),
      'last_name' => $profile_wrapper->commerce_customer_address->last_name->value(),
      'company' => $profile_wrapper->commerce_customer_address->organisation_name->value(),
      'address' => $profile_wrapper->commerce_customer_address->thoroughfare->value(),
      'city' => $profile_wrapper->commerce_customer_address->locality->value(),
      'state' => $profile_wrapper->commerce_customer_address->administrative_area->value(),
      'zip' => $profile_wrapper->commerce_customer_address->postal_code->value(),
      'country' => $profile_wrapper->commerce_customer_address->country->value(),
    ),
  ));

  $entity->save('commerce_fulfillment_package', $entity);
}
/**
 * @param $form
 * @param $form_state
 * @param $order_id
 * @return array
 *
 * Implements hook_form
 * Shows form for printing packing slips
 */
function commerce_fulfillment_packing_slip_form($form, $form_state, $order_id){

  $form = array();

  $form['commerce_fulfillment_order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order_id,
  );

  $form['commerce_fulfillment_title'] = array(
    '#markup' => '<h2>' . t('Print Packing Slip:') . '</h2>',
  );

  $form['commerce_fulfillment_package'] = array(
    '#type' => 'select',
    '#title' => t('Select Package:'),
    '#options' => commerce_fulfillment_get_array($order_id, 'false', 'package_in_order'),
  );

  $form['commerce_fulfillment_print'] = array(
    '#type' => 'submit',
    '#value' => t('Print Packing Slip')
  );

  return $form;

}

/**
 * @param $form
 * @param $form_state
 * redirects the user to the packing slip
 */
function commerce_fulfillment_packing_slip_form_submit($form, &$form_state){
  $argument = commerce_fulfillment_get_array($form_state['values']['commerce_fulfillment_order_id'], $form_state['values']['commerce_fulfillment_package'], 'package_in_order');
  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['values']['commerce_fulfillment_order_id'] .'/commerce_fulfillment/' . $argument . '/packing_slip';
}


/**
 * @param $package_id
 * @return string|void
 *
 * Tests to make sure user only selected one package.
 * If user selected more than one package redirects the user back to the package page.
 */
function commerce_fulfillment_packing_slip_view($package_id){
  if (strstr($package_id, ',')) {
    drupal_set_message(t('Please select only one Package'), 'error');
    $package_id = strtok($package_id, ',');
    $package = entity_load('commerce_fulfillment_package', FALSE, array('package_id'=>$package_id));
    $package = $package[$package_id];
    $order_id = $package->order_id;
    drupal_goto('admin/commerce/orders/'.$order_id.'/commerce_fulfillment/orders');
  }
  else {
    commerce_fulfillment_packing_slip_content($package_id);
  }
}

/**
 * @param $shipment_id
 * @return string|void
 *
 * Tests to make sure user only selected one shipment.
 * If user selected more than one shipment redirects the user back to the shipments page.
 */
function commerce_fulfillment_shipping_label_view($shipment_id){
  if (strstr($shipment_id, ',')) {
    drupal_set_message(t('Please select only one Shipment'), 'error');
    $shipment_id = strtok($shipment_id, ',');
    $shipment = entity_load('commerce_fulfillment_shipment', FALSE, array('shipment_id'=>$shipment_id));
    $shipment = $shipment[$shipment_id];
    $order_id = $shipment->order_id;
    drupal_goto('admin/commerce/orders/'.$order_id.'/commerce_fulfillment/shipments');
  }
  else {
    commerce_fulfillment_shipping_label_content($shipment_id);
  }
}

/**
 * @param $package_id
 *
 * The page to show and print the packing slip
 *
 */
function commerce_fulfillment_packing_slip_content($package_id){
    $package_selected = entity_load('commerce_fulfillment_package', FALSE, array('package_id' => $package_id));
    $package_wrapper = entity_metadata_wrapper('commerce_fulfillment_package', $package_selected[$package_id]);
    $order_id = $package_selected[$package_id]->order_id;
    $packages = entity_load('commerce_fulfillment_package', FALSE, array('order_id' => $order_id));
    $products = $package_wrapper->commerce_fulfillment_line_items->value();

    $count = count($packages);

    $image = variable_get('commerce_fulfillment_logo', NULL);

    if ($image !== NULL) {
      $image_obj = file_load($image);

      if (isset($image_obj->uri)) {
        $image_obj_url = file_create_url($image_obj->uri);
      }

    } else {
      $image_obj_url = NULL;
    }
    $company = variable_get('commerce_fulfillment_company');
    $address = variable_get('commerce_fulfillment_address');
    $phone = variable_get('commerce_fulfillment_phone');

    $page = commerce_fulfillment_page_prerequisite();

    $output = theme('packing_slip', array(
      'company' => $company,
      'address' => $address,
      'phone' => $phone,
      'image' => $image_obj_url,
      'products' => $products,
      'count' => $count,
      'page' => $page,
    ));

    drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
    drupal_send_headers();
    print $output;
//  $output = t('<table width = "700" border = "1"><tr><td width = "20%"><img src = "'. $image_obj_url .'"></td>');
//  $output .= t('<td width = "80%"><p></p><strong>Company Info.: <br></strong>&emsp;'
//    . variable_get('commerce_fulfillment_company', NULL) . '</p>&emsp;'
//    . variable_get('commerce_fulfillment_address', NULL) . '<br>&emsp;'
//    . variable_get('commerce_fulfillment_phone', NULL)
//    . '</td></tr>');
//
//  $output .= t('<tr><td style = vertical-align:top align = "top" height = "500" colspan ="2">
//        <table cellpadding = "15" width = "700" >');
//  $output .= t('<th align = "left"> Product Code:</th><th align = "left"> Quantity</th>
//    <th align = "left"> Unit Price:</th><th align = "left"> Total Price:</th>');
//
//  foreach($products as $product){
//
//    $product_wrapper = entity_metadata_wrapper('commerce_line_item', $product);
//    $prices[0] = $product_wrapper->commerce_unit_price->amount->value();
//    $prices[1] = $product_wrapper->commerce_total->amount->value();
//
//    $length = strlen($prices[0]);
//    $prices[0] = substr_replace($prices[0], '.', $length - 2, 0);
//    $length = strlen($prices[1]);
//    $prices[1] = substr_replace($prices[1], '.', $length - 2, 0);
//
//    //@TODO possibly break this up into a few $output .= lines, makes it a bit easier to read and the speed is the same
//    $output .= t('<tr><td>' . $product->line_item_label . '</td><td>' . $product->quantity .'</td><td>' .
//      $prices[0] . ' ' . $product_wrapper->commerce_unit_price->currency_code->value() . '</td><td>' . $prices[1] . ' '
//      . $product_wrapper->commerce_total->currency_code->value() . '</td></tr>');
//  }
//
//  $output .= t('</table></td></tr>');
//  $output .= t('<tr><td colspan = "2"><strong>Package ID: </strong>' . $package_wrapper->package_id->value()
//    . '<br><strong>Package:</strong> 1 of ' . $count . '</td>');
//  $output.= t('</tr></table>');
//
//  print $output;
}

/**
 * @param $order
 * @return string
 *
 * Builds the shipments page
 *
 */
function commerce_fulfillment_shipment_content($order) {

  $order_id = $order->order_id;

  $shipments = entity_load('commerce_fulfillment_shipment', FALSE, array('order_id'=>$order_id));
  dpm($shipments);

  $get_form = drupal_get_form('commerce_fulfillment_create_shipment_form', $order_id);
  $output = drupal_render($get_form);
  $output .= '<h2>' . t('Packages:') . '</h2>';
  $output .= views_embed_view('commerce_fulfillment_packages', 'package_shipment_block', $order_id);
  $output .= '<h2>' . t('Shipments:') . '</h2>';
  $output .= views_embed_view('commerce_fulfillment_shipments', 'default', $order_id);

  return $output;
}

/**
 * @param $form
 * @param $form_state
 * @param $order_id
 * @return array
 * implements hook form
 * implements functionality to select which shipment to print shipping labels for.
 *
 */
function commerce_fulfillment_shipping_label_print_form($form, &$form_state, $order_id){

  $form = array();

  $form['commerce_fulfillment_order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order_id,
  );

  $form['commerce_fulfillment_title'] = array(
    '#markup'=>'<h2>' . t('Print Shipping Label:') . '</h2>',
  );

  $form['commerce_fulfillment_shipment'] = array(
    '#type' => 'select',
    '#title' => t('Select Shipment:'),
    '#options' => commerce_fulfillment_get_array($order_id, 'false', 'shipment_in_order'),
  );

  $form['commerce_fulfillment_print'] = array(
    '#type' => 'submit',
    '#value' => t('Print Shipping Label')
  );

  return $form;
}

/**
 * @param $form
 * @param $form_state
 * redirects the user to the shipping label page
 */
function commerce_fulfillment_shipping_label_print_form_submit($form, &$form_state){
  $order_id = $form_state['values']['commerce_fulfillment_order_id'];
  $argument = commerce_fulfillment_get_array($order_id, $form_state['values']['commerce_fulfillment_shipment'], 'shipment_in_order');
  $form_state['redirect'] = 'admin/commerce/orders/' . $order_id .'/commerce_fulfillment/' . $argument . '/shipping_label';
}

/**
 * @param $shipment_id
 * @return string
 *
 * Prints the shipping label on its own page. Takes in the shipment_id and loads the shipment.
 * Prints Company name, address, phone # and logo from the administrator page.
 *
 */
function commerce_fulfillment_shipping_label_content($shipment_id){
  $shipment_arr = entity_load('commerce_fulfillment_shipment', FALSE, array('shipment_id' => $shipment_id));
  $shipment = $shipment_arr[$shipment_id];
  $image = variable_get('commerce_fulfillment_logo', NULL);

  if($image !== NULL) {
    $image_obj = file_load($image);

    if (isset($image_obj->uri)) {
      $image_obj_url = file_create_url($image_obj->uri);
    }
  }
  else {
    $image_obj_url = NULL;
  }

  $company = variable_get('commerce_fulfillment_company');
  $address = variable_get('commerce_fulfillment_address');
  $phone = variable_get('commerce_fulfillment_phone');

  $page = commerce_fulfillment_page_prerequisite();

  $output = theme('shipping_label', array(
    'company' => $company,
    'address' => $address,
    'phone' => $phone,
    'image' => $image_obj_url,
    'shipment' => $shipment,
    'page' => $page,
  ));

  drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
  drupal_send_headers();
  print $output;
//  $output = t('<table border = "1px solid black" height = "350" width = "700"><tr><td height = "50%">
//    <strong> From: </Strong>' . variable_get('commerce_fulfillment_company', NULL) . '<br>&emsp;&emsp;&emsp;'
//    . variable_get('commerce_fulfillment_address', NULL)
//    . '<br>&emsp;&emsp;&emsp;' . variable_get('commerce_fulfillment_phone', NULL) . '</td>');
//  $output .= t('<td align = "center"><img src = "' . $image_obj_url .  '")></td></tr>');
//  $output .= t('<tr><td height = "50%"><strong>To: </strong>' . $shipment->shipping_info['firstName'] . ' '
//    . $shipment->shipping_info['lastName'] . '<br>'
//    . $shipment->shipping_info['address'] . '<br>' . $shipment->shipping_info['city'] . ', '
//    . $shipment->shipping_info['state'] . ', ' . $shipment->shipping_info['country'] . '<br>'
//    . $shipment->shipping_info['zip'] . '</td>');
//  $output .= t('<td width = "50%"><strong>Order ID: </strong>' . $shipment->order_id
//    . '<br><strong>Tracking Number: </strong>' . $shipment->tracking_number . '<br><strong>Shipping Method: </strong>'
//    . $shipment->method['method'] . '<br>' . '</tr></table>');
//
//  print $output;
}

/**
 * Prepare the array for the template with common details
 * @see more _print_var_generator() in print.pages.inc of the Print module
 */
function commerce_fulfillment_page_prerequisite(){
  global $base_url, $language;
  $page = array();
  $page['language'] = $language->language;
  $page['head'] = drupal_get_html_head();
  $page['title'] = '';
  $page['scripts'] = drupal_get_js();
  $page['favicon'] = '';
  // if there is a custom css file for this page
  // drupal_add_css(drupal_get_path('module', 'mymodule') . '/css/mylogin.css');
  $page['css'] = drupal_get_css();
  $page['message'] = drupal_get_messages();
  $page['footer_scripts'] = drupal_get_js('footer');

  return $page;
}
/**
 * @param $form
 * @param $form_state
 * @param $order_id
 * @return array
 *
 * Implements hook_form
 * Implements form to create a new shipment object
 * Allows user to set the tracking number if desired
 *
 */
function commerce_fulfillment_create_shipment_form($form, &$form_state,$order_id){

  $form = array();

  $form['commerce_fulfillment_order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order_id,
  );

  $form['commerce_fulfillment_title'] = array(
    '#markup'=>'<h2>' . t('Create a New Shipment:') . '</h2>',
  );

  $form['commerce_fulfillment_tracking_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter tracking number:'),
    '#size' => 20,
    '#length' => 255,
    '#required' => false,
  );

  $form['commerce_fulfillment_create'] = array(
    '#type' => 'submit',
    '#value' => t('Create Shipment')
  );

  return $form;
}

function commerce_fulfillment_create_shipment_form_submit(&$form, &$form_state){

  $order_id = $form_state['values']['commerce_fulfillment_order_id'];
  $order = commerce_order_load($order_id);
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $profile = commerce_customer_profile_load($order_wrapper->commerce_customer_shipping->profile_id->value());

  $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);

  $entity = entity_create('commerce_fulfillment_shipment', array(
    'order_id' => $order->order_id,
    'status' => 'Pending',
    'tracking_number' => $form_state['values']['commerce_fulfillment_tracking_number'],
  ));

  $entity->save('commerce_fulfillment_shipment', $entity);

  //commerce_fulfillment_shipping will not save from the entity_create. So we create an Entity Metadata Wrapper to do the job.
  $shipment_wrapper = entity_metadata_wrapper('commerce_fulfillment_shipment', $entity);
  $shipping_array = array(
    $profile_wrapper->commerce_customer_address->first_name->value(),
    $profile_wrapper->commerce_customer_address->last_name->value(),
    $profile_wrapper->commerce_customer_address->organisation_name->value(),
    $profile_wrapper->commerce_customer_address->thoroughfare->value(),
    $profile_wrapper->commerce_customer_address->locality->value(),
    $profile_wrapper->commerce_customer_address->administrative_area->value(),
    $profile_wrapper->commerce_customer_address->postal_code->value(),
    $profile_wrapper->commerce_customer_address->country->value(),
    array(
      date('ymd'),
    ),
  );

  $shipment_wrapper->commerce_fulfillment_shipping->set($shipping_array);
  $shipment_wrapper->save();
}
