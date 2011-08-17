<?php
function erp_client_form($form, $form_state, $client=null){

	if(is_null($client)){
		$client = array();
	}
	
	$form["name"] = array(
		"#type"=>"textfield",
		"#title"=>t("Name"),
		"#description"=>t("Client name"),
		"#required"=>true
	);
	
	$form["desc"] = array(
		"#type"=>"textarea",
		"#title"=>t("Description"),
		"#required"=>false
	);
	
	field_attach_form("erp_client", $client, $form, $form_state);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#value"=>t("Save"),
		"#name"=>"btn-name"
	);
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#value"=>t("Cancel"),
		"#name"=>"btn-cancel"
	);
	
	return $form;
	
}

function erp_client_form_validate($form, &$form_state){
	
}

function erp_client_form_submit($form, &$form_state){
	
}