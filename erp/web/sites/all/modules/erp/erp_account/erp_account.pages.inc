<?php
function erp_account_view($client, $account){
	
	$out = "<h2>".t("ERP Account View")."</h2>";
	$out .= "<pre>".print_r($client, true)."</pre>";
	$out .= "<pre>".print_r($account, true)."</pre>";
	
	return $out;
	
}

function erp_account_list_form($form, $form_state, $client = null){
	
	$header = array(
		"name"=>t("Name"),
		"description"=>t("Description")
	);
	$accounts = erp_account_load_multiple(false);
	foreach($accounts as $account){
		$options[$account->id] = array(
			"name"=>l($account->name,"erp/client/{$client->id}/account/{$account->id}"),
			"description"=>$account->description
		);
	}
	$options = array();
	$form["accounts"] = array(
		"#type"=>"tableselect",
		"#multiple"=>true,
		"#header"=>$header,
		"#options"=>$options
	);
	
	$actions = actions_get_all_actions();
	$options = array();
	foreach($actions as $aid=>$action){
		if($action["type"] == "erp_account"){
			$options[$aid] = t($action["label"]);
		}
	}
	$form["operation"] = array(
		"#type"=>"select",
		"#title"=>t("Operation"),
		"#required"=>true,
		"#options"=>$options
	);
	
	$form["actions"]["submit"] = array(
		"#type"=>"submit",
		"#value"=>t("Submit"),
		"#name"=>"btn-name"
	);
	
	return $form;
	
}

function erp_account_list_form_validate($form, &$form_state){

}

function erp_account_list_form_submit($form, &$form_state){

}

function erp_account_form($form, $form_state, $client = null, $account = null){

	if(is_null($client)){
		watchdog("erp_account", "Parameter 'client' must not be null", array(), WATCHDOG_ERROR);
		drupal_set_message(t("An error has occurred that will prevent you from saving this account."),"error");
	}
	
	$form["client"] = array(
		"#type"=>"value",
		"#value"=>$client
	);
	
	$types = AccountDb::account_types_get_all();
	$options = array();
	foreach($types as $type){
		$options[$type["id"]] = $type["name"];
	}
	$form["type"] = array(
		"#type"=>"select",
		"#required"=>true,
		"#options"=>$options
	);
	
	if(!is_null($account)){
		$account = new stdClass();
	}
	
	$form["account"] = array(
		"#type"=>"value",
		"#value"=>$account
	);
	
	$form["name"] = array(
		"#type"=>"textfield",
		"#title"=>t("Name"),
		"#required"=>true
	);
	
	$form["desc"] = array(
		"#type"=>"textfield",
		"#title"=>t("Description"),
		"#required"=>true
	);
	
	$form["code"] = array(
		"#type"=>"textfield",
		"#title"=>t("Account code"),
		"#required"=>false
	);
	
	$options = array();
	$form["parent"] = array(
		"#type"=>"select",
		"#title"=>t("Parent Account"),
		"#required"=>false,
		"#options"=>$options
	);
	
	field_attach_form("erp_account", $account, $form, $form_state);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#value"=>t("Save"),
		"#name"=>"btn-name",
		"#disabled"=>is_null($client)
	);
	
	return $form;
	
}

function erp_account_form_validate($form, &$form_state){
	
}

function erp_account_form_submit($form, &$form_state){

}