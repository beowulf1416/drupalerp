<?php
function erp_contact_form($form, $form_state, $client=null, $contact=null){
	
	if(is_null($contact)){
		$contact = new stdClass();
	}
	
	$form["contact"] = array(
		"#type"=>"value",
		"#value"=>$contact
	);
	
	$form["name-first"] = array(
		"#type"=>"textfield",
		"#title"=>t("First name"),
		"#required"=>true,
		"#maxlength"=>100
	);
	
	$form["name-last"] = array(
		"#type"=>"textfield",
		"#title"=>t("Last name"),
		"#required"=>true,
		"#maxlength"=>100
	);
	
	$form["name-middle"] = array(
		"#type"=>"textfield",
		"#title"=>t("Middle"),
		"#required"=>false,
		"#maxlength"=>100
	);
	
	$form["name-prefix"] = array(
		"#type"=>"textfield",
		"#title"=>t("Prefix"),
		"#required"=>false,
		"#maxlength"=>50
	);
	
	$form["name-suffix"] = array(
		"#type"=>"textfield",
		"#title"=>t("Sufffix"),
		"#required"=>false,
		"#maxlength"=>50
	);
	
	field_attach_form("erp_contact", $contact, $form, $form_state);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#value"=>t("Save"),
		"#name"=>"btn-save"
	);
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#value"=>t("Cancel"),
		"#name"=>"btn-cancel"
	);
	
	return $form;
	
}

function erp_contact_form_validate($form, &$form_state){
	
	if($form_state["triggering_element"]["#name"] != "btn-cancel"){
		entity_form_field_validate("erp_contact", $form, $form_state);
	}

}

function erp_contact_form_submit($form, &$form_state){

	switch($form_state["triggering_element"]["#name"]){
		case "btn-save":{
			
			
			
			break;
		}
	}
	
}

function erp_contact_delete_form($form, $form_state, $client=null, $contact=null){
	
	$form["msg"] = array(
		"#markup"=>"<div class=\"form-msg-warning\">".
			t("Are you sure you want to delete %contact?",
				array("%contact"=>"//TODO")).
				"</div><!-- .form-msg-warning -->"
	);
	
	$form["actions"]["yes"] = array(
		"#type"=>"submit",
		"#value"=>t("Yes"),
		"#name"=>"btn-yes"
	);
	
	$form["actions"]["no"] = array(
		"#type"=>"submit",
		"#value"=>t("No"),
		"#name"=>"btn-no"
	);
	
	return $form;
	
}

function erp_contact_delete_form_validate($form, &$form_state){

}

function erp_contact_delete_form_submit($form, &$form_state){

}