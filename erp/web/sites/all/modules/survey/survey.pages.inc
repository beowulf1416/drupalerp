<?php
function survey_list(){
	
	$surveys = SurveyDb::survey_get_all();
	return theme("survey_list",array(
		"surveys"=>$surveys
	));
	
}

function survey_form($form, $form_state, $survey = null){

	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["page"] = array(
		"#tree"=>true
	);
	
	return $form;
	
}

function survey_form_validate($form, &$form_state){

}

function survey_form_submit($form, &$form_state){

}

function survey_edit_form($form, $form_state, $survey = null){
	
	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["title"] = array(
		"#type"=>"textfield",
		"#title"=>t("Title"),
		"#required"=>true,
		"#default_value"=>$survey == null ? "" : $survey->title
	);
	
	$form["desc"] = array(
		"#type"=>"textarea",
		"#title"=>t("Description"),
		"#default_value"=>$survey == null ? "" : $survey->description
	);
	
	if(isset($survey)){
		$form["link-survey-pages-add"] = array(
			"#markup"=>l("Add survey page","survey/{$survey->id}/page/add")
		);
	}
	
	$options = array();
	$form["questions"] = array(
		"#type"=>"tableselect",
		"#header"=>array(
			"page_id"=>t("Page"),
			"text"=>t("Text"),
			"type"=>t("Type")
		),
		"#options"=>$options,
		"#empty"=>isset($survey) ? l("Add question","survey/{$survey->id}/question/add") : t("No questions")
	);
	
	if(isset($survey)){
		$form["closed"] = array(
			"#type"=>"checkbox",
			"#title"=>t("This survey is closed"),
			"#required"=>false,
			"#default_value"=>false
		);
	}
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#name"=>"btn-cancel",
		"#value"=>t("Cancel")
	);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#name"=>"btn-save",
		"#value"=>t("Save")
	);
	
	return $form;
	
}

function survey_edit_form_validate($form, &$form_state){

}

function survey_edit_form_submit($form, &$form_state){

	switch($form_state["triggering_element"]["#name"]){
		case "btn-save":{
			
			try {
				$title = $form_state["values"]["title"];
				$desc = $form_state["values"]["desc"];
				$survey_id = survey_add($title, $desc);
				drupal_set_message(t("Survey saved"));
				
				$form_state["redirect"] = "survey/{$survey_id}/edit";
			}catch(Exception $e){
				watchdog_exception("survey", $e);
				drupal_set_message(t("An error occurred while saving the survey."));
			}
			break;
		}
	}
	
}

function survey_page_form($form, $form_state, $survey = null, $page = null){
	
	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["page"] = array(
		"#type"=>"value",
		"#value"=>$page
	);
	
	$form["title"] = array(
		"#type"=>"textfield",
		"#title"=>t("Title"),
		"#required"=>true,
		"#default_value"=>isset($page) ? $page->title : ""
	);
	
	$form["weight"] = array(
		"#type"=>"textfield",
		"#title"=>t("Weight"),
		"#required"=>true,
		"#default_value"=>isset($page) ? $page->weight : 0
	);
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#name"=>"btn-cancel",
		"#value"=>t("Cancel")
	);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#name"=>"btn-save",
		"#value"=>t("Save")
	);
	
	return $form;
	
}

function survey_page_form_validate($form, &$form_state){

}

function survey_page_form_submit($form, &$form_state){

	$survey = $form_state["values"]["survey"];
	switch($form_state["triggering_element"]["#name"]){
		case "btn-save":{
			
			try {
				$survey_id = $survey->id;
				$title = $form_state["values"]["title"];
				$weight = $form_state["values"]["weight"];
				$page_id = survey_page_add($survey_id, $title, $weight);
				drupal_set_message(t("Saved survey page"));
				
				$form_state["redirect"] = "survey/{$survey->id}/edit";
			}catch(Exception $e){
				watchdog_exception("survey", $e);
				drupal_set_message(t("An error occured while trying to save the survey page."),"error");
				
				$form_state["redirect"] = "survey/{$survey->id}/page/add";
			}
			
			break;
		}
		case "btn-cancel":{
			
			$form_state["redirect"] = "survey/{$survey->id}/edit";
			
			break;
		}
	}
	
}

function survey_question_form($form, $form_state, $survey = null, $question = null){
	
	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["question"] = array(
		"#type"=>"value",
		"#value"=>$question
	);
	
	$form["name"] = array(
		"#type"=>"textfield",
		"#title"=>t("Name"),
		"#required"=>true,
		"#default_value"=>""
	);
	
	$form["text"] = array(
		"#type"=>"textarea",
		"#title"=>t("Text"),
		"#required"=>true,
		"#default_value"=>""
	);

	$types = array();
	$results = SurveyDb::get_question_types();
	foreach($results as $result){
		$types[$result["id"]] = t($result["name"]);
	}
	$form["type"] = array(
		"#type"=>"select",
		"#title"=>t("Type"),
		"#required"=>true,
		"#options"=>$types,
		"#default_value"=>0,
		"#ajax"=>array(
			"callback"=>"survey_question_form_type_callback",
			"wrapper"=>"survey-type-ajax-result-wrapper"
		)
	);
	
	$type_id = isset($form_state["values"]["type"]) ? $form_state["values"]["type"] : QTYPE_TEXT;
	switch($type_id){
		case QTYPE_TEXT:{
			$form["example-type-text"] = array(
				"#markup"=>isset($form_state["values"]["text"]) ? "<p>".$form_state["values"]["text"]."</p>" : "<p>Why is the sky blue?</p>",
				"#prefix"=>"<div class=\"question-example\>",
				"#suffix"=>"</div><!-- .question-example -->"
			);
			break;
		}
		case QTYPE_YES_NO:{
			$form["example-type-yes-no"] = array(
				"#markup"=>isset($form_state["values"]["text"]) ? "<p>".$form_state["values"]["text"]."</p>" : "<p>Why is the sky blue?</p>",
				"#prefix"=>"<div class=\"question-example\>",
				"#suffix"=>"</div><!-- .question-example -->"
			);
			break;
		}
		case QTYPE_TRUE_FALSE:{
			$form["example-type-true-false"] = array(
				"#markup"=>isset($form_state["values"]["text"]) ? "<p>".$form_state["values"]["text"]."</p>" : "<p>Why is the sky blue?</p>",
				"#prefix"=>"<div class=\"question-example\>",
				"#suffix"=>"</div><!-- .question-example -->"
			);
			break;
		}
		case QTYPE_SELECT_SINGLE:{
			$form["example-type-yes-no"] = array(
				"#markup"=>isset($form_state["values"]["text"]) ? "<p>".$form_state["values"]["text"]."</p>" : "<p>Why is the sky blue?</p>",
				"#prefix"=>"<div class=\"question-example\>",
				"#suffix"=>"</div><!-- .question-example -->"
			);
			break;
		}
		case QTYPE_SELECT_MULTI:{
			$form["example-type-yes-no"] = array(
				"#markup"=>isset($form_state["values"]["text"]) ? "<p>".$form_state["values"]["text"]."</p>" : "<p>Why is the sky blue?</p>",
				"#prefix"=>"<div class=\"question-example\>",
				"#suffix"=>"</div><!-- .question-example -->"
			);
			break;
		}
		default:{
			$form["example-type-text"] = array(
				"#markup"=>isset($form_state["values"]["text"]) ? "<p>".$form_state["values"]["text"]."</p>" : "<p>Why is the sky blue?</p>",
				"#prefix"=>"<div class=\"question-example\>",
				"#suffix"=>"</div><!-- .question-example -->"
			);
			break;
		}
	}
	
	$options = array();
	$pages = survey_page_get_all($survey->id);
	foreach($pages as $page){
		$options[$page["id"]] = $page["title"];
	}
	$form["page"] = array(
		"#type"=>"select",
		"#title"=>t("Page"),
		"#options"=>$options,
		"#required"=>true
	);
	$form["link-page-add"] = array(
		"#markup"=>l("Add page","survey/{$survey->id}/page/add"),
		"#prefix"=>"<div class=\"link-page-add-wrapper\">",
		"#suffix"=>"</div><!-- .link-page-add-wrapper -->"
	);
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#name"=>"btn-cancel",
		"#value"=>"Cancel"
	);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#name"=>"btn-save",
		"#value"=>"Save"
	);
	
	return $form;
	
}

function survey_question_form_type_callback($form, $form_state){
	
	return $form["type-params"];
	
}

function survey_question_form_validate($form, &$form_state){
	
}

function survey_question_form_submit($form, &$form_state){

}

function survey_question_selection_form($form, $form_state, $survey = null, $question = null, $title = null, $value = null){

	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["question"] = array(
		"#type"=>"value",
		"#value"=>$question
	);
	
	$form["title"] = array(
		"#type"=>"textfield",
		"#title"=>t("Title"),
		"#required"=>true,
		"#default_value"=>$title
	);
	
	$form["value"] = array(
		"#type"=>"textfield",
		"#title"=>t("Value"),
		"#required"=>true,
		"#default_value"=>$value
	);
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#name"=>"btn-cancel",
		"#value"=>t("Cancel")
	);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#name"=>"btn-save",
		"#value"=>t("Save")
	);
	
}

function survey_question_selection_form_validate($form, &$form_state){
	
}

function survey_question_selection_form_submit($form, &$form_state){

}