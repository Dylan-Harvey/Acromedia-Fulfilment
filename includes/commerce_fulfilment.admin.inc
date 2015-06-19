<?php
/**
 *
 * @return array
 * Implements hook_form
 * Creates an admin page form
 * Will use this data to print packing slips and shipping labels
 */
function commerce_fulfilment_admin_form(){
  $form = array();
  $form['commerce_fulfilment_title'] = array(
    '#markup' => '<h2>Admin Page</h2>'
  );

  //Creates seperate information fields

  $form['commerce_fulfilment_company_information']=array(
    '#type'=>'fieldset',
    '#title'=>t("Enter your company's information below"),
    '#description'=>'Enter information to use on Packing Slips and Shipping Labels.'
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
  $form['commerce_fulfilment_company_information']['commerce_fulfilment_boxtype'] = array(
    '#type'=>'textfield',
    '#title'=>t('Add Package Type:'),
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
 * Implements hook hook_form_validate
 * Validates information entered into the form
 */
function commerce_fulfilment_admin_form_validate($form, &$form_state){
}
/** Implements hook hook_form_submit
 *  Does not override system_settings_form submit.
 *  Just handles submission of the package_type variable
 */

function commerce_fulfilment_admin_form_submit($form, &$form_state){
  //gets global variable package_type
  $package_type = variable_get('commerce_fulfilment_package_type');

  //initializes global variable package_type
  if(!isset($package_type)){
    $package_type = array();
    $package_type[] = $form_state['values']['commerce_fulfilment_boxtype'];
    variable_set('commerce_fulfilment_package_type', $package_type);
  }
  //appends global variable package_type
  else{
    $package_type[] = $form_state['values']['commerce_fulfilment_boxtype'];
    variable_set('commerce_fulfilment_package_type', $package_type);
  }
}
