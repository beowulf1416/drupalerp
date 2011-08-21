<?php
function erp_account_view($client, $account){
	
	return "<h2>".t("ERP Account View")."</h2>";
	
}

function erp_account_form($form, $form_state, $client = null, $account = null){
	
	$form["client"] = array(
		"#type"=>"value",
		"#value"=>$client
	);
	
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
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#value"=>t("Save"),
		"#name"=>"btn-name"
	);
	
	return $form;
	
}

function erp_account_form_validate($form, &$form_state){
	
}

function erp_account_form_submit($form, &$form_state){

}