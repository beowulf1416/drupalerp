<?php
/**
 * variables:
 * $surveys
 */
?>
<div class="surveys-wrapper">
<?php 
	$header = array(
		array("data"=>t("Title")),
		array("data"=>t("Description"))
	);
	$rows = array();
	foreach($surveys as $survey){
		$rows[] = array(
			array("data"=>l($survey["title"],"survey/{$survey["id"]}")),
			array("data"=>$survey["description"])
		);
	}
	print(theme("table",array(
		"header"=>$header,
		"rows"=>$rows
	)));
	
?>
</div><!-- .surveys-wrapper -->
<?php 
print(l("Add survey","survey/add"));
?>