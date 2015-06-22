<?php

/**
 * @param $form
 * @param $form_state
 * @return array
 * Implements hook_form
 * Creates an admin page form
 * Will use this data to print packing slips and shipping labels
 */
function commerce_fulfilment_admin_form($form, &$form_state){

  $form = array();

//  $form['#tree'] = TRUE;
  $form['commerce_fulfilment_title'] = array(
    '#markup' => '<h2>Admin Page</h2>'
  );

  //Creates seperate information fields
  $form['commerce_fulfilment_company_information']=array(
    '#type'=>'fieldset',
    '#title'=>t("Enter your company's information below"),
    '#description'=>'Enter information to use on Packing Slips and Shipping Labels.',
    '#prefix' => '<div id="boxtype-field-wrapper">',
    '#suffix' => '</div>',
  );

  //Textfields
  $form['commerce_fulfilment_company_information']['commerce_fulfilment_company'] = array(
    '#type'=>'textfield',
    '#title'=>t('Company name:'),
    '#default_value' => variable_get('commerce_fulfilment_company', NULL),
    '#required' => TRUE,
  );
  $form['commerce_fulfilment_company_information']['commerce_fulfilment_address'] = array(
    '#type'=>'textfield',
    '#title'=>t('Address:'),
    '#default_value' => variable_get('commerce_fulfilment_address', NULL),
    '#required' => TRUE,
  );
  $form['commerce_fulfilment_company_information']['commerce_fulfilment_phone'] = array(
    '#type'=>'textfield',
    '#title'=>t('Company Phone Number:'),
    '#default_value' => variable_get('commerce_fulfilment_phone', NULL),
    '#required' => TRUE,
  );

  if(!isset($form_state['num_boxtypes'])){
    $form_state['num_boxtypes'] = 1;
  }

  for($i = 0; $i < $form_state['num_boxtypes']; $i++){
    $form['commerce_fulfilment_company_information']['commerce_fulfilment_boxtype'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('Add Package Type:'),
    );
  }
  $form['commerce_fulfilment_company_information']['add_boxtype'] = array(
    '#type' => 'submit',
    '#value' => t('Add Another Package Type'),
    '#submit' => array('commerce_fulfilment_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'commerce_fulfilment_add_more_callback',
      'wrapper' => 'boxtype-field-wrapper',
      'method' => 'replace',
    ),
  );

  //Allows user to upload company logo
  $form['#attributes']['enctype'] = 'multipart/form-data';

  $form['commerce_fulfilment_company_information']['commerce_fulfilment_logo']=array(
    '#type'=>'managed_file',
    '#title'=>t('Upload your company logo:'),
    '#default_value' => variable_get('commerce_fulfilment_logo', NULL),
    '#upload_location' => 'public://logo/'
  );
  $form = system_settings_form($form);
  $form['#submit'][] = 'commerce_fulfilment_admin_form_submit';

  return $form;
}

/**
 * @param $form
 * @param $form_state
 * Implements hook hook_form_validate
 * Validates information entered into the form
 */
function commerce_fulfilment_admin_form_validate($form, &$form_state){
}

/**
 *  @param $form
 *  @param $form_state
 *  Implements hook hook_form_submit
 *  Does not override system_settings_form submit.
 *  Just handles submission of the package_type variable
 */

function commerce_fulfilment_admin_form_submit($form, &$form_state){

  //gets global variable package_type
  $package_type = variable_get('commerce_fulfilment_package_type', NULL);

  //initializes global variable package_type
  if(!isset($package_type)) {
    $package_type = array();
  }

  //appends global variable package_type
  for($i = $form_state['num_boxtypes']-1; $i>=0; $i--) {
    $package_type[] = $form_state['values'][$i];
  }
  variable_set('commerce_fulfilment_package_type', $package_type);

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
function commerce_fulfilment_add_more_callback($form, &$form_state){
  return $form['commerce_fulfilment_company_information'];
}

/**
 * @param $form
 * @param $form_state
 *
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function commerce_fulfilment_add_more_add_one($form, &$form_state) {
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
function commerce_fulfilment_package_content($order){
  $order_wrapper = entity_metadata_wrapper( 'commerce_order', $order);
  $order_id = $order_wrapper->order_id->value();

  $packages = entity_load('commerce_fulfilment_package', FALSE, array('order_id'=>$order_id));

  $output = t('<table><tr><td style = vertical-align:top>');
  $output .= t('<h2>' . t('Products in Order ' .$order_id . ':') . '</h2>');
  $output .= views_embed_view('commerce_fulfilment_line_items', 'default', $order_id);
  $output .= t('</td><td style = vertical-align:top><h2>Created Packages for This Order:</h2>');

  //outputs each package
  foreach ($packages as $package) {
    $package_wrapper = entity_metadata_wrapper('commerce_fulfilment_package', $package);
    $output .= t('<h3>Package ' . $package_wrapper->package_id->value() . ':</h3>');
    $output .= t('<p>&emsp;<strong>Type: </strong>' . $package_wrapper->box_type->value() . '</p>');
    $output .= t('<h4>Products in Package:</h4>');
    $line_items = array();

    //creates an array of line_items that are in the specific package
    foreach($package_wrapper->commerce_fulfilment_line_items->value() as $line_item){
      $line_items[] = $line_item;
    }

    //outputs each line_item that are in the specific package
    foreach ($line_items as $line_item) {
      $output .= t('<p><strong>Product ID: </strong>' . $line_item->line_item_id
        . '</p><p>&emsp;<strong>Product Code: </strong>' . $line_item->line_item_label . '</p>');
      $output .= t('<p>&emsp;<strong>Quantity: </strong>' . $line_item->quantity . '</p>');
    }

    //outputs form to interact with the package
    $get_form = drupal_get_form('commerce_fulfilment_manipulate_package_form_' . $package_wrapper->package_id->value(),
      array('0' => $package_wrapper->package_id->value(), '1' => $order_id));
    $output .= drupal_render($get_form);
  }

  //outputs forms to create a package and to print the packing slip for the selected package
  $output.= t('</td></tr><tr>');
  $get_form = drupal_get_form('commerce_fulfilment_create_package_form', $order_id);
  $output .= t('<td>' . drupal_render($get_form) . '</td>');

  $get_form = drupal_get_form('commerce_fulfilment_packing_slip_form', $order_id);
  $output .= t('<td>' . drupal_render($get_form) . '</td>');

  $output .= t('</tr></table>');

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
function commerce_fulfilment_create_package_form($form, &$form_state,$order_id){

  $package_type = variable_get('commerce_fulfilment_package_type', array(0 => 'No Package Type set.'));

  $form = array();

  $form['commerce_fulfilment_order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order_id,
  );
  $form['commerce_fulfilment_title'] = array(
    '#markup'=>'<h2>Create a New Package:</h2>'
  );
  $form['commerce_fulfilment_package_type'] = array(
    '#type' => 'select',
    '#title' => 'Select package type: ',
    '#options' => $package_type,
    '#default_value' => 'default',
  );
  $form['commerce_fulfilment_create'] = array(
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
function commerce_fulfilment_create_package_form_submit($form, &$form_state){
  $package_type = variable_get('commerce_fulfilment_package_type', array(0 => 'No Package Type set.'));

  //create an order entity wrapper
  $order_id = $form_state['values']['commerce_fulfilment_order_id'];
  $order = commerce_order_load($order_id);
  $order_wrapper= entity_metadata_wrapper('commerce_order', $order);

  //create a package entity wrapper
  $profile_id = $order_wrapper->commerce_customer_billing->profile_id->value();
  $profile = commerce_customer_profile_load($profile_id);
  $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);

  //create the package entity
  $entity = entity_create('commerce_fulfilment_package', array(
    'order_id' => $order_wrapper->order_id->value(),
    'box_type' => $package_type[$form_state['values']['commerce_fulfilment_package_type']],
    'commerce_fulfilment_line_items' => NULL,
    'shipping_info'=> array(
      'firstName' => $profile_wrapper->commerce_customer_address->first_name->value(),
      'lastName' => $profile_wrapper->commerce_customer_address->last_name->value(),
      'company' => $profile_wrapper->commerce_customer_address->organisation_name->value(),
      'address' => $profile_wrapper->commerce_customer_address->thoroughfare->value(),
      'city' => $profile_wrapper->commerce_customer_address->locality->value(),
      'state' => $profile_wrapper->commerce_customer_address->administrative_area->value(),
      'zip' => $profile_wrapper->commerce_customer_address->postal_code->value(),
      'country' => $profile_wrapper->commerce_customer_address->country->value(),
    ),
  ));

  //insert entity into database
  $entity->save('commerce_fulfilment_package', $entity);
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
function commerce_fulfilment_packing_slip_form($form, $form_state, $order_id){
  $package_arr = entity_load('commerce_fulfilment_package', FALSE, array('order_id' => $order_id));

  $form = array();
  $form['commerce_fulfilment_order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order_id,
  );
  $form['commerce_fulfilment_title'] = array(
    '#markup'=>'<h2>Print Packing Slip:</h2>'
  );
  $form['commerce_fulfilment_package'] = array(
    '#type' => 'select',
    '#title' => t('Select Package:'),
    '#options' => commerce_fulfilment_get_package_array($package_arr),
  );
  $form['commerce_fulfilment_print'] = array(
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
function commerce_fulfilment_packing_slip_form_submit($form, &$form_state){
  $package_arr = entity_load('commerce_fulfilment_package', FALSE, array('order_id' => $form_state['values']['commerce_fulfilment_order_id']));
  $argument = commerce_fulfilment_get_package_array($package_arr, (int)$form_state['values']['commerce_fulfilment_package']);

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['values']['commerce_fulfilment_order_id'] .'/commerce_fulfilment/' . $argument . '/packing_slip';
}

/**
 * @param $package_id
 *
 * The page to show and print the packing slip
 *
 */
function commerce_fulfilment_packing_slip_content($package_id){
  $package_selected = entity_load('commerce_fulfilment_package', FALSE, array('package_id' => $package_id));
  $package_wrapper = entity_metadata_wrapper('commerce_fulfilment_package', $package_selected[$package_id]);
  $order_id = $package_selected[$package_id]->order_id;
  $packages = entity_load('commerce_fulfilment_package', FALSE, array('order_id' => $order_id));
  $products = $package_wrapper->commerce_fulfilment_line_items->value();
  $count = 0;

  foreach($packages as $package){
    $count++;
  }
  $image = variable_get('commerce_fulfilment_logo', NULL);

  if($image !== NULL) {
    $image_obj = file_load($image);

    if (isset($image_obj->uri)) {
      $image_obj_url = file_create_url($image_obj->uri);
    }

  }
  else {
    $image_obj_url = NULL;
  }

  $output = t('<table width = "700" border = "1"><tr><td width = "20%"><img src = "'. $image_obj_url .'"></td>');
  $output .= t('<td width = "80%"><p></p><strong>Company Info.: <br></strong>&emsp;'
    . variable_get('commerce_fulfilment_company', NULL) . '</p>&emsp;'
    . variable_get('commerce_fulfilment_address', NULL) . '<br>&emsp;'
    . variable_get('commerce_fulfilment_phone', NULL)
    . '</td></tr>');

  $output .= t('<tr><td style = vertical-align:top align = "top" height = "500" colspan ="2">
        <table cellpadding = "15" width = "700" >');
  $output .= t('<th align = "left"> Product Code:</th><th align = "left"> Quantity</th>
    <th align = "left"> Unit Price:</th><th align = "left"> Total Price:</th>');

  foreach($products as $product){

    $product_wrapper = entity_metadata_wrapper('commerce_line_item', $product);
    $prices[0] = $product_wrapper->commerce_unit_price->amount->value();
    $prices[1] = $product_wrapper->commerce_total->amount->value();

    $length = strlen($prices[0]);
    $prices[0] = substr_replace($prices[0], '.', $length - 2, 0);
    $length = strlen($prices[1]);
    $prices[1] = substr_replace($prices[1], '.', $length - 2, 0);

    $output .= t('<tr><td>' . $product->line_item_label . '</td><td>' . $product->quantity .'</td><td>' .
      $prices[0] . ' ' . $product_wrapper->commerce_unit_price->currency_code->value() . '</td><td>' . $prices[1] . ' '
      . $product_wrapper->commerce_total->currency_code->value() . '</td></tr>');
  }

  $output .= t('</table></td></tr>');
  $output .= t('<tr><td colspan = "2"><strong>Package ID: </strong>' . $package_wrapper->package_id->value()
    . '<br><strong>Package:</strong> 1 of ' . $count . '</td>');
  $output.= t('</tr></table>');
  print $output;
}

/**
 * @param $order
 * @return string
 *
 * Builds the shipments page
 *
 */
function commerce_fulfilment_shipment_content($order) {

  $order_id = $order->order_id;

  $packages = entity_load('commerce_fulfilment_package', FALSE, array('order_id'=>$order_id));
  $packages_id = array();
  $shipments = entity_load('commerce_fulfilment_shipment', FALSE, array('order_num'=>$order_id));

  $output = t('<table border = "0"><tr><td style = vertical-align:top><h2>Created Packages for This Order:</h2>');

  foreach ($packages as $package) {
    $package_wrapper = entity_metadata_wrapper('commerce_fulfilment_package', $package);
    $package_id = $package_wrapper->package_id->value();
    $packages_id[] = $package_wrapper->package_id->value();
    $output .= t('<h3>Package ' . $package_id . ':</h3>');
    $output .= t('<h4>Products in Package:</h4>');
    $line_items = array();

    foreach($package_wrapper->commerce_fulfilment_line_items->value() as $line_item){
      $line_items[] = $line_item;
    }

    foreach ($line_items as $line_item) {
      $line_item = entity_metadata_wrapper('commerce_line_item', $line_item);
      $output .= t('<p><strong>Product ID: </strong>' .$line_item->line_item_id->value()
        . '</p><p><strong>Product Code: </strong>' . $line_item->line_item_label->value() . '</p>');
    }
  }
  $output .= t('</td><td style = vertical-align:top><h2>Created Shipments for This Order:</h2>');

  foreach ($shipments as $shipment) {
    $shipment_wrapper = entity_metadata_wrapper('commerce_fulfilment_shipment', $shipment);
    $shipment_id = $shipment_wrapper->shipment_id->value();
    $output .= t('<h3>Shipment ' . $shipment_id . ':</h3>');
    $output .= t('<p>&emsp;<strong>Tracking Number: </strong>' . $shipment_wrapper->tracking_number->value() . '</p>');
    $output .= t('<p>&emsp;<strong>Date Created: </strong>'
      . date("Y M d", strtotime($shipment->dates['created'])) . '</p>');
    $output .= t('<p><strong>&emsp;Address: </strong>' . $shipment->shipping_info['address'] .
      ', ' . $shipment->shipping_info['city'] . ' ' . $shipment->shipping_info['state'] .
      ', ' . $shipment->shipping_info['country'] . ' ' . $shipment->shipping_info['zip'] . '</p>');

    $output .= t('<h4>&emsp;Packages in Shipment:</h4>');
    $package_ids = $shipment_wrapper->package_num->value();

    if($package_ids !== NULL) {

      foreach ($package_ids as $package_id) {

        if ($package_id !== NULL) {
          $output .= t('<p>&emsp;&emsp;<strong>Package ID: </strong>' . $package_id
            . '</p>');
        }
      }
    }
    $args = array($shipment_id);
    $get_form = (drupal_get_form('commerce_fulfilment_manipulate_shipment_form_' . $shipment_id, $args));
    $output .= drupal_render($get_form);
  }
  $output .= t('</td></tr><tr><td>');
  $get_form = drupal_get_form('commerce_fulfilment_create_shipment_form', $order_id);
  $output .= drupal_render($get_form) . '</td>';

  $get_form = drupal_get_form('commerce_fulfilment_shipping_label_print_form', $order_id);
  $output .= t('<td>' . drupal_render($get_form) . '</td></tr></table>');

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
function commerce_fulfilment_shipping_label_print_form($form, &$form_state, $order_id){
  $shipment_arr = entity_load('commerce_fulfilment_shipment', FALSE, array('order_num' => $order_id));

  $form = array();
  $form['commerce_fulfilment_order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order_id,
  );
  $form['commerce_fulfilment_title'] = array(
    '#markup'=>'<h2>Print Shipping Label:</h2>'
  );
  $form['commerce_fulfilment_shipment'] = array(
    '#type' => 'select',
    '#title' => t('Select Shipment:'),
    '#options' => commerce_fulfilment_get_shipment_array($shipment_arr),
  );
  $form['commerce_fulfilment_print'] = array(
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
function commerce_fulfilment_shipping_label_print_form_submit($form, &$form_state){
  $order_id = $form_state['values']['commerce_fulfilment_order_id'];
  $shipment_arr = entity_load('commerce_fulfilment_shipment', FALSE, array('order_num' => $order_id));
  $argument = commerce_fulfilment_get_shipment_array($shipment_arr, (int)$form_state['values']['commerce_fulfilment_shipment']);
  $form_state['redirect'] = 'admin/commerce/orders/' . $order_id .'/commerce_fulfilment/' . $argument . '/shipping_label';
}

/**
 * @param $shipment_id
 * @return string
 *
 * Prints the shipping label on its own page. Takes in the shipment_id and loads the shipment.
 * Prints Company name, address, phone # and logo from the administrator page.
 *
 */
function commerce_fulfilment_shipping_label_content($shipment_id){
  $shipment_arr = entity_load('commerce_fulfilment_shipment', FALSE, array('shipment_id' => $shipment_id));
  $shipment = $shipment_arr[$shipment_id];
  $image = variable_get('commerce_fulfilment_logo', NULL);

  if($image !== NULL) {
    $image_obj = file_load($image);

    if (isset($image_obj->uri)) {
      $image_obj_url = file_create_url($image_obj->uri);
    }
  }
  else {
    $image_obj_url = NULL;
  }
  $output = t('<table border = "1px solid black" height = "350" width = "700"><tr><td height = "50%">
    <strong> From: </Strong>' . variable_get('commerce_fulfilment_company', NULL) . '<br>&emsp;&emsp;&emsp;'
    . variable_get('commerce_fulfilment_address', NULL)
    . '<br>&emsp;&emsp;&emsp;' . variable_get('commerce_fulfilment_phone', NULL) . '</td>');
  $output .= t('<td align = "center"><img src = "' . $image_obj_url .  '")></td></tr>');
  $output .= t('<tr><td height = "50%"><strong>To: </strong>' . $shipment->shipping_info['firstName'] . ' '
    . $shipment->shipping_info['lastName'] . '<br>'
    . $shipment->shipping_info['address'] . '<br>' . $shipment->shipping_info['city'] . ', '
    . $shipment->shipping_info['state'] . ', ' . $shipment->shipping_info['country'] . '<br>'
    . $shipment->shipping_info['zip'] . '</td>');
  $output .= t('<td width = "50%"><strong>Order ID: </strong>' . $shipment->order_num
    . '<br><strong>Tracking Number: </strong>' . $shipment->tracking_number . '<br><strong>Shipping Method: </strong>'
    . $shipment->method['method'] . '<br>' . '</tr></table>');
  print $output;
}

/**
 * @param $shipments
 * @param bool $index
 * @return array
 * returns an array of shipment ids if there is no index.
 * If there is an index returns the selected shipment ID
 *
 */
function commerce_fulfilment_get_shipment_array($shipments, $index = FALSE) {
  $shipment_id = array();
  $count = 0;

  //Creates an array of package id's for the select box.
  foreach ($shipments as $shipment) {
    $shipment_wrapper = entity_metadata_wrapper('commerce_fulfilment_shipment', $shipment);
    $shipment_id[$count] = $shipment_wrapper->shipment_id->value();
    $count++;
  }

  if($index !== FALSE && is_int($index) && $index <= count($shipment_id)) {
    return $shipment_id[$index];
  }

  return $shipment_id;
}

/**
 * @param $packages
 * @param bool $index
 * @return array
 * Gets an array of package_ids to display in the commerce_fulfilment_manipulate_package_form_builder
 */
function commerce_fulfilment_get_package_array($packages, $index = FALSE){
  $package_id = array();
  $count = 0;

  //Creates an array of package id's for the select box.
  foreach ($packages as $package) {
    $package_wrapper = entity_metadata_wrapper('commerce_fulfilment_package', $package);
    $package_id[$count] = $package_wrapper->package_id->value();
    $count++;
  }

  if($index !== FALSE && is_int($index) && $index <= count($package_id)) {
    return $package_id[$index];
  }

  return $package_id;
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

function commerce_fulfilment_create_shipment_form($form, &$form_state,$order_id){

  $form = array();
  $form['commerce_fulfilment_order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order_id,
  );
  $form['commerce_fulfilment_title'] = array(
    '#markup'=>'<h2>Create a New Shipment:</h2>'
  );
  $form['commerce_fulfilment_tracking_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter tracking number:'),
    '#size' => 20,
    '#length' => 255,
    '#required' => false,
  );
  $form['commerce_fulfilment_create'] = array(
    '#type' => 'submit',
    '#value' => t('Create Shipment')
  );

  return $form;
}

function commerce_fulfilment_create_shipment_form_submit(&$form, &$form_state){

  $order_id = $form_state['values']['commerce_fulfilment_order_id'];
  $order = commerce_order_load($order_id);
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $profile = commerce_customer_profile_load($order_wrapper->commerce_customer_shipping->profile_id->value());
  $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);

  $entity = entity_create('commerce_fulfilment_shipment', array(
    'order_num' => $order->order_id,
    'status' => 'pending',
    'packages' => '',
    'dates' => array(
      'created' => date('ymd'),
    ),
    'shipping_info' => array(
      'firstName' => $profile_wrapper->commerce_customer_address->first_name->value(),
      'lastName' => $profile_wrapper->commerce_customer_address->last_name->value(),
      'company' => $profile_wrapper->commerce_customer_address->organisation_name->value(),
      'address' => $profile_wrapper->commerce_customer_address->thoroughfare->value(),
      'city' => $profile_wrapper->commerce_customer_address->locality->value(),
      'state' => $profile_wrapper->commerce_customer_address->administrative_area->value(),
      'zip' => $profile_wrapper->commerce_customer_address->postal_code->value(),
      'country' => $profile_wrapper->commerce_customer_address->country->value(),
    ),
    'tracking_number' => $form_state['values']['commerce_fulfilment_tracking_number'],
  ));

  $entity->save('commerce_fulfilment_shipment', $entity);
}
