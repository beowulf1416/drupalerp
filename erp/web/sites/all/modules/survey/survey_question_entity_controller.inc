<?php
class SurveyQuestionEntityController extends DrupalDefaultEntityController {
	
	protected function attachLoad(&$queried_entities, $revision_id = false){
		
		foreach($queried_entities as $q){
			$question_id = $q->id;
			$q->dependencies = survey_questions_dependencies($q->survey_id, $q->id);
		}
		
	}
	
}