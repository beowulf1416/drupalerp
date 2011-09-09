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
		"#value"=>isset($form_state["values"]["survey"]) ?
			$form_state["values"]["survey"] : $survey
	);
	
	$step = isset($form_state["values"]["step"]) ? $form_state["values"]["step"] : 1;
	$form["step"] = array(
		"#type"=>"value",
		"#value"=>$step
	);
	
	$form["pages"] = array(
		"#tree"=>true
	);
	
	$pages = SurveyDb::get_pages($survey->id);
	$max_step = count($pages);
	$form["max-step"] = array(
		"#type"=>"value",
		"#value"=>$max_step
	);
	
	for($i=0; $i<count($pages); $i++){
		$page = $pages[$i];
		$page_id = $page["id"];
		
		$form["pages"]["page-{$page_id}"] = array(
			"#tree"=>true
		);
		
		$questions = SurveyDb::get_questions($survey->id, $page_id);
		if($i == $step - 1){

			if(isset($page["description"])){
				$form["pages"]["page-{$page_id}"]["desc"] = array(
					"#markup"=>t($page["description"]),
					"#prefix"=>"<div id=\"page-desc-{$page_id}\">",
					"#suffix"=>"</div><!-- #page-desc-{$page_id} -->"
				);
			}
			
			foreach($questions as $q){
				$question_id = $q["id"];
				$type_id = $q["type_id"];
				$form["pages"]["page-{$page_id}"]["question-{$question_id}"] = 
					_question_form_element($form_state, $page, $q);
			} // foreach
			
		} else {
			
			foreach($questions as $q){
				$question_id = $q["id"];
				$type_id = $q["type_id"];
				$form["pages"]["page-{$page_id}"]["question-{$question_id}"] = array(
					"#type"=>"value",
					"#value"=>isset($form_state["values"]["pages"]["page-{$page_id}"]["question-{$question_id}"]) ?
						$form_state["values"]["pages"]["page-{$page_id}"]["question-{$question_id}"] : null
				);
			} // foreach
			
		} // if
		
	} // for
	
	if($step != 1){
		$form["actions"]["prev"] = array(
			"#type"=>"submit",
			"#name"=>"btn-previous",
			"#value"=>t("Previous")
		);
	}

	if($step != $max_step){
		$form["actions"]["next"] = array(
			"#type"=>"submit",
			"#name"=>"btn-next",
			"#value"=>t("Next")
		);
	}
	
	if($step == $max_step){
		$form["actions"]["finish"] = array(
			"#type"=>"submit",
			"#name"=>"btn-finish",
			"#value"=>t("Finish")
		);
	}
		
	return $form;
	
}

function _question_form_element(&$form_state, $page, $question){
	
	$question_id = $question["id"];
	$type_id = $question["type_id"];
	$default_value = isset($form_state["values"]["pages"]["page-{$page["id"]}"]["question={$question_id}"]) ?
		$form_state["values"]["pages"]["page-{$page["id"]}"]["question={$question_id}"] : "";
	switch($type_id){
		case QTYPE_TRUE_FALSE:{
			return array(
				"#type"=>"radios",
				"#title"=>t($question["text"]),
				"#description"=>isset($question["description"]) ?
					$question["description"] : "",
				"#required"=>$question["is_required"] != Q_NOT_REQ_VALUE,
				"#options"=>array(
					ANSWER_TRUE=>t("True"),
					ANSWER_FALSE=>t("False")
				),
				"#default_value"=>$default_value
			);
			break;
		}
		case QTYPE_YES_NO:{
			return array(
				"#type"=>"radios",
				"#title"=>t($question["text"]),
				"#description"=>isset($question["description"]) ?
					$question["description"] : "",
				"#required"=>$question["is_required"] != Q_NOT_REQ_VALUE,
				"#options"=>array(
					ANSWER_YES=>t("Yes"),
					ANSWER_NO=>t("No")
				),
				"#default_value"=>$default_value
			);
			break;
		}
		default:{
			return array(
				"#type"=>"textfield",
				"#title"=>t($question["text"]),
				"#description"=>isset($question["description"]) ?
					$question["description"] : "",
				"#required"=>$question["is_required"] != Q_NOT_REQ_VALUE,
				"#default_value"=>$default_value
			);
		}
	}
	
}

function survey_form_validate($form, &$form_state){

}

function survey_form_submit($form, &$form_state){

	$step = isset($form_state["values"]["step"]) ? $form_state["values"]["step"] : 1;
	$max_step = isset($form_state["values"]["max-step"]) ? $form_state["values"]["max-step"] : 2;
	switch($form_state["triggering_element"]["#name"]){
		case "btn-next":{
			$step += 1;
			$form_state["values"]["step"] = $step;
			$form_state["rebuild"] = true;
			break;
		}
		case "btn-previous":{
			$step -= 1;
			$form_state["values"]["step"] = $step;
			$form_state["rebuild"] = true;
			break;
		}
		case "btn-finish":{
			_save_survey_response($form_state);
			break;
		}
	} // switch
	
}

function _save_survey_response(&$form_state){
	
	// TODO _save_survey_response
	
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
	
	$form["questions"] = array(
		"#tree"=>true
	);
	
	$options = array();
	$types = survey_types_get_all();
	$pages = isset($survey) ? survey_page_get_all($survey->id) : array();
	$questions = isset($survey) ? survey_questions_get_all($survey->id) : array();
	foreach($questions as $q){
		$options[$q["id"]] = array(
			"weight"=>$q["weight"],
			"page_id"=>$pages[$q["page_id"]]["title"],
			"name"=>isset($survey) ? l($q["name"],"survey/{$survey->id}/question/{$q["id"]}/edit") : $q["name"],
			"text"=>$q["text"],
			"type"=>$types[$q["type_id"]]["name"]
		);
	}
	$form["questions"] = array(
		"#type"=>"tableselect",
		"#header"=>array(
			"name"=>t("Name"),			
			"text"=>t("Text"),
			"type"=>t("Type"),
			"page_id"=>t("Page"),
			"weight"=>t("Weight"),
		),
		"#options"=>$options,
		"#empty"=>isset($survey) ? l("Add question","survey/{$survey->id}/question/add") : t("No questions")
	);
	
	if(isset($survey)){
		$form["link-question-add"] = array(
			"#markup"=>l("Add question","survey/{$survey->id}/question/add"),
			"#prefix"=>"<div id=\"link-question-add-wrapper\">",
			"#suffix"=>"</div><!-- #link-question-add-wrapper -->"
		);
	}
	
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
	
	$form["desc"] = array(
		"#type"=>"textarea",
		"#title"=>t("Description"),
		"#required"=>false,
		"#default_value"=>isset($page) ? $page->description : ""
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
				$desc = $form_state["values"]["desc"];
				$weight = $form_state["values"]["weight"];
				$page_id = survey_page_add($survey_id, $title, $desc, $weight);
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
		"#default_value"=>isset($question) ? $question->name : ""
	);
	
	$form["text"] = array(
		"#type"=>"textarea",
		"#title"=>t("Text"),
		"#required"=>true,
		"#default_value"=>isset($question) ? $question->text : ""
	);
	
	$form["desc"] = array(
		"#type"=>"textarea",
		"#title"=>t("Description"),
		"#required"=>false,
		"#default_value"=>isset($question) ? $question->description : ""
	);
	
	$form["required"] = array(
		"#type"=>"checkbox",
		"#title"=>t("Required"),
		"#required"=>false,
		"#default_value"=>isset($question) ? $question->is_required : false
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
		"#default_value"=>isset($question) ? $question->type_id : QTYPE_TEXT,
		"#ajax"=>array(
			"callback"=>"survey_question_form_type_callback",
			"wrapper"=>"survey-type-ajax-result-wrapper"
		)
	);
	
	$form["type-extras"] = array(
		"#tree"=>true,
		"#prefix"=>"<div id=\"survey-type-ajax-result-wrapper\">",
		"#suffix"=>"</div><!-- #survey-type-ajax-result-wrapper -->"
	);
	
	_question_type_form_elements($form, $form_state, $survey, $question);
	
	$options = array();
	$pages = survey_page_get_all($survey->id);
	foreach($pages as $page){
		$options[$page["id"]] = $page["title"];
	}
	$form["page"] = array(
		"#type"=>"select",
		"#title"=>t("Page"),
		"#options"=>$options,
		"#required"=>true,
		"#default_value"=>isset($question) ? $question->page_id : 0
	);
	$form["link-page-add"] = array(
		"#markup"=>l("Add page","survey/{$survey->id}/page/add"),
		"#prefix"=>"<div class=\"link-page-add-wrapper\">",
		"#suffix"=>"</div><!-- .link-page-add-wrapper -->"
	);
	
	$form["weight"] = array(
		"#type"=>"textfield",
		"#title"=>t("Weight"),
		"#required"=>false,
		"#default_value"=>isset($question) ? $question->weight : 0
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

function _question_type_form_elements(&$form, $form_state, $survey, $question){
	
	$type_id = isset($form_state["values"]["type"]) ? $form_state["values"]["type"] : $question->type_id;
	switch($type_id){
		case QTYPE_SELECT_SINGLE:{
			
			$form["type-extras"]["question-{$question->id}"] = array(
				"#tree"=>true
			);
			
			$options = survey_question_options_get_all($survey->id, $question->id);
			$form["type-extras"]["question-{$question->id}"]["selections"] = array(
				"#type"=>"tableselect",
				"#header"=>array(
					"name"=>t("Name"),
					"value"=>t("Value")
				),
				"#options"=>$options,
				"#empty"=>t("No options set"),
				"#required"=>false
			);
			
			$form["type-extras"]["question-{$question->id}"]["link-option-add"] = array(
				"#markup"=>l("Add option","survey/{$survey->id}/question/{$question->id}/options/add"),
			);
			break;
		}
	}
	
}

function survey_question_form_type_callback($form, $form_state){
	
	return $form["type-extras"];
	
}

function survey_question_form_validate($form, &$form_state){
	
}

function survey_question_form_submit($form, &$form_state){

	switch($form_state["triggering_element"]["#name"]){
		case "btn-save":{
			
			$survey = $form_state["values"]["survey"];
			$question = isset($form_state["values"]["question"]) ? $form_state["values"]["question"] : null;
			
			try {
				$name = isset($form_state["values"]["name"]) ? $form_state["values"]["name"] : "";
				$text = isset($form_state["values"]["text"]) ? $form_state["values"]["text"] : "";
				$desc = isset($form_state['values']["desc"]) ? $form_state["values"]["desc"] : "";
				$type_id = isset($form_state["values"]["type"]) ? $form_state["values"]["type"] : QTYPE_TEXT;
				$page_id = isset($form_state["values"]["page"]) ? $form_state["values"]["page"] : 0;
				$weight = isset($form_state["values"]["weight"]) ? $form_state["values"]["weight"] : 0;
				$is_required = isset($form_state["values"]["required"]) ? $form_state["values"]["required"] : false;
				
				if(isset($question) && is_object($question) && isset($question->id)){
					survey_question_update($question->id, $name, $text, $desc, $type_id, $page_id, $weight, $is_required);
				} else {
					$question_id = survey_question_add($survey->id, $name, $text, $desc, $type_id, $page_id, $weight, $is_required);
				}
				
				drupal_set_message(t("Saved survey question"));
				$form_state["redirect"] = "survey/{$survey->id}/edit";
			}catch(Exception $e){
				watchdog_exception("survey", $e);
				drupal_set_message(t("An error occured while trying to save the survey question."),"error");
				
				if(isset($question) && is_object($question) && isset($question->id)){
					$form_state["redirect"] = "survey/{$survey->id}/question/{$question->id}/edit";
				} else {
					$form_state["redirect"] = "survey/{$survey->id}/question/add";
				}
			}
			
			break;
		}
		case "btn-cancel":{
			$form_state["redirect"] = "survey/{$survey->id}/edit";
			break;
		}
	}
	
}

function survey_question_option_form($form, $form_state, $survey = null, $question = null, $option = null){

	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["question"] = array(
		"#type"=>"value",
		"#value"=>$question
	);
	
	$form["option"] = array(
		"#type"=>"value",
		"#value"=>$option
	);
	
	$form["title"] = array(
		"#type"=>"textfield",
		"#title"=>t("Title"),
		"#required"=>true,
		"#default_value"=>""
	);
	
	$form["value"] = array(
		"#type"=>"textfield",
		"#title"=>t("Value"),
		"#required"=>true,
		"#default_value"=>""
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

function survey_question_option_form_validate($form, &$form_state){
	
}

function survey_question_option_form_submit($form, &$form_state){

}