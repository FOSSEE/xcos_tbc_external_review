<?php

function xcos_tbc_external_review_eligibility_test_form($form, $form_state, $question_id=0) {
        $form = array();
        global $user;
        
        $query = "
            SELECT * FROM xcos_external_review_eligibility_answers
            WHERE uid = {:uid} AND question_id = {:question_id} AND reset = 0 
        ";
        $result = db_query($query,array(':uid'=>$user->uid, ':question_id'=> $question_id));
        $row = $result->fetchObject();
        
        $form["wrapper"] = array(
            "#type" => "fieldset",
            "#title" => t("Eligibility Test"),
            "#prefix" => "<div id='eligibility-form-wrapper'>",
            "#suffix" => "</div>",
        );
        $form["wrapper"]["ncf"] = array(
            "#type" => "radios",
            "#title" => t("Naming convention followed? (Checklist point 1, 2 and 4)"),
            "#options" => array(
                t("Yes"),
                t("No"),
            ),
            "#required" => TRUE,
            "#default_value" => $row->ncf
        );
        $form["wrapper"]["eit"] = array(
            "#type" => "radios",
            "#title" => t("Error in textbook mentioned as comment?"),
            "#options" => array(
                t("Yes"),
                t("No"),
                t("Not required and answer matches with the textbook"),
            ),
            "#required" => TRUE,
            "#default_value" => $row->eit
        );
        $form["wrapper"]["sua"] = array(
            "#type" => "radios",
            "#title" => t("Symbols used are appropiate?"),
            "#options" => array(
                t("Yes"),
                t("No"),
            ),
            "#required" => TRUE,
            "#default_value" => $row->sua
        );
        $form["wrapper"]["sfu"] = array(
            "#type" => "radios",
            "#title" => t("Scilab functions used?"),
            "#options" => array(
                t("Yes"),
                t("No"),
            ),
            "#required" => TRUE,
            "#default_value" => $row->sfu
        );
        $form["wrapper"]["aci"] = array(
            "#type" => "radios",
            "#title" => t("Appropriate comments included as mentioned in checklist point 7?"),
            "#options" => array(
                t("Yes"),
                t("No"),
            ),
            "#required" => TRUE,
            "#default_value" => $row->aci
        );
        $form["wrapper"]["auu"] = array(
            "#type" => "radios",
            "#title" => t("Appropriate units used as given in the textbook?"),
            "#options" => array(
                t("Yes"),
                t("No"),
            ),
            "#required" => TRUE,
            "#default_value" => $row->auu
        );
        $form["wrapper"]["teo"] = array(
            "#type" => "radios",
            "#title" => t("Typographical errors in output and/or comments?"),
            "#options" => array(
                t("Yes"),
                t("No"),
            ),
            "#required" => TRUE,
            "#default_value" => $row->teo
        );
        $form["wrapper"]["mit"] = array(
            "#type" => "radios",
            "#title" => t("The code can be approved if the same example is solved using Matlab in the textbook."),
            "#options" => array(
                t("Yes"),
                t("No"),
            ),
            "#required" => TRUE,
            "#default_value" => $row->mit
        );
        $form["wrapper"]["agl"] = array(
            "#type" => "radios",
            "#title" => t("Axes of graph need not be labeled"),
            "#options" => array(
                t("Yes"),
                t("No"),
            ),
            "#required" => TRUE,
            "#default_value" => $row->agl
        );
        $form["wrapper"]["question_id"] = array(
            "#type" => "hidden",
            "#value" => $question_id
        );
        $form["wrapper"]["submit"] = array(
            "#type" => "submit",
            "#value" => "Submit and Continue."
        );
        return $form;
    }

    function xcos_tbc_external_review_eligibility_test_form_submit($form, &$form_state) {
        global $user;
        
        $v = $form_state["values"];
        
        /* checking the timer and status of current user */
        $query = "
            SELECT * FROM xcos_external_review_eligibility_timings
            WHERE uid = {:uid}
        ";
        $result = db_query($query,array(':uid'=> $user->uid));
        $num_rows = $result->rowCount(); //either 1 or 0
        if($num_rows){
            /* now checking whether the test is still open */
            $row =$result->fetchObject();
            $start_date = new DateTime($row->start_date);
            $current_date = new DateTime();
            $since_start = $start_date->diff($current_date);
            $minutes_remaining = 24 - $since_start->i;
            $seconds_remaining = 60 - $since_start->s;
            if($minutes_remaining >= 0) {
            } else {
                drupal_goto("xcos-tbc-external-review/eligibility-test/end");
            }
        }
        
        $query = "
            SELECT id FROM xcos_external_review_eligibility_answers
            WHERE 
            uid =  {:uid} AND question_id = {:question_id}
        ";
        $result = db_query($query,array(':uid'=> $user->uid,':question_id'=> $v['question_id'] ));
        $num_rows = $result->rowCount();
        if($num_rows) {
           /* $query = "
                UPDATE xcos_external_review_eligibility_answers
                SET
                ncf = {$v['ncf']}, eit = {$v['eit']}, sua = {$v['sua']},
                sfu = {$v['sfu']}, aci = {$v['aci']}, auu = {$v['auu']},
                teo = {$v['teo']}, mit = {$v['mit']}, agl = {$v['agl']}
                WHERE
                uid = {$user->uid} AND question_id = {$v['question_id']}
                ";
            $result = db_query($query);*/
            $query = db_update('xcos_external_review_eligibility_answers');
            $query->fields(array(
              'ncf' => $v['ncf'],
              'eit' => $v['eit'],
              'sua' => $v['sua'],
              'sfu' => $v['sfu'],
              'aci' => $v['aci'], 
              'auu' => $v['auu'],
              'teo' => $v['teo'],
              'mit' => $v['mit'],
              'agl' => $v['agl'],
            ));
            $query->condition('uid', $user->uid);          
            $query->condition('question_id', $v['question_id']);
            $result = $query->execute();
            //var_dump('ok');
           // die;            
        } else {
         
            /*$query = "
                INSERT INTO xcos_external_review_eligibility_answers
                (
                    uid, question_id, 
                    ncf, eit, sua,
                    sfu, aci, auu,
                    teo, mit, agl
                )
                VALUES
                (
                    {$user->uid}, {$v['question_id']},
                    {$v['ncf']}, {$v['eit']}, {$v['sua']},
                    {$v['sfu']}, {$v['aci']}, {$v['auu']},
                    {$v['teo']}, {$v['mit']}, {$v['agl']}
                )
            ";
            $result = db_query($query);
        }*/
        
        $query = "
                INSERT INTO xcos_external_review_eligibility_answers
                (
                    uid, question_id, 
                    ncf, eit, sua,
                    sfu, aci, auu,
                    teo, mit, agl
                )
                VALUES
                (  {:uid}, {:question_id}, 
                   {:ncf}, {:eit}, {:sua},
                   {:sfu}, {:aci}, {:auu},
                   {:teo}, {:mit}, {:agl}
                   
                )
            ";
           
            $args = array(
                          ':uid' => $user->uid ,
                          ':question_id' => $v['question_id'],
                          ':ncf' => $v['ncf'],
                          ':eit' => $v['eit'],
                          ':sua' => $v['sua'],
                          ':sfu' => $v['sfu'],
                          ':aci' => $v['aci'], 
                          ':auu' => $v['auu'],
                          ':teo' => $v['teo'],
                          ':mit' => $v['mit'],
                          ':agl' => $v['agl'],
                            
                    );
                    $result = db_query($query, $args);
        }
        
        $question_id = $v["question_id"];
        if($question_id == 5) {
            // end the test
            drupal_goto("xcos-tbc-external-review/eligibility-test/end");
        } else {
            drupal_goto("xcos-tbc-external-review/eligibility-test/question/".($question_id+1));
        }
    }

    function xcos_tbc_external_review_eligibility_test_all($action="", $question_id=0){
        global $user;
        $page_content = "";
        $minutes_remaining = 25;
        $seconds_remaining = 0;
        
        /* checking if a valid question */
        if ($question_id > 5 || $question_id < 0) {
            drupal_set_message("Wrong question number.", "error");
            drupal_goto("xcos-tbc-external-review/eligibility-test/question/1");
        }
        
        if($action == "question" && $question_id) {
            /* checking the timer and status of current user */
            $query = "
                SELECT * FROM xcos_external_review_eligibility_timings
                WHERE uid = {:uid}
            ";
            $result = db_query($query, array(':uid'=> $user->uid ));
            $num_rows = $result->rowCount(); //either 1 or 0
            if($num_rows){
                /* now checking whether the test is still open */
                $row = $result->fetchObject();
                $start_date = new DateTime($row->start_date);
                $current_date = new DateTime();
                $since_start = $start_date->diff($current_date);
                $minutes_remaining = 24 - $since_start->i;
                $seconds_remaining = 60 - $since_start->s;
                if($minutes_remaining >= 0) {
                } else {
                    drupal_goto("xcos-tbc-external-review/eligibility-test/end");
                }
            } else {
                /* inserting a new entry into timings table */
                $current_date = new DateTime();
                $current_date = $current_date->format('Y-m-d H:i:s');
                $query = "
                    INSERT INTO xcos_external_review_eligibility_timings
                    (uid, status, start_date)
                    VALUES
                    ({:uid}, 1, {:current_date})
                ";
                $result = db_query($query, array(':uid'=> $user->uid, ':current_date' => $current_date ));
            }
            
            /* getting the questions */
            $query = "
                SELECT * FROM xcos_external_review_eligibility_questions
                WHERE id = {:question_id}
            ";
            $result = db_query($query, array(':question_id'=>$question_id));
            $row = $result->fetchObject();
            $page_content .= "<div id='timer'>";
                $page_content .= "<span id='minutes'></span>m: ";
                $page_content .= "<span id='seconds'></span>s";
            $page_content .= "</div>";
            $page_content .= "<h4>";
                if ($question_id != 1) {
                    $page_content .= l("<Prev - ", "xcos-tbc-external-review/eligibility-test/question/" . ($question_id-1));
                }
                $page_content .= "Question {$question_id} of 5";
                if ($question_id != 5) {
                    $page_content .= l(" - Next>", "xcos-tbc-external-review/eligibility-test/question/" . ($question_id+1));
                }
            $page_content .= "</h4><br>";
            $page_content .= "<div id='example-name'>Example file name: <strong>{$row->example_name}</strong></div><br>";
            $page_content .= "<div class='well'><pre>{$row->code}</pre></div>";
            
            $xcos_tbc_external_review_eligibility_test_form = drupal_get_form("xcos_tbc_external_review_eligibility_test_form", $question_id);
            $page_content .= drupal_render($xcos_tbc_external_review_eligibility_test_form);
            
            /* hidden fields with minutes_remaining and seconds_remaining */
            $page_content .= "<input id='minutes_remaining' value='{$minutes_remaining}' type='hidden'>";
            $page_content .= "<input id='seconds_remaining' value='{$seconds_remaining}' type='hidden'>";
        } else if($action == "end") {
            $query = "
                UPDATE xcos_external_review_eligibility_timings
                SET status = 2
                WHERE uid = {:uid}
            ";
            $result = db_query($query, array(':uid'=> $user->uid));
            drupal_set_message("Eligibility Test completed successfully. We will get back to you soon.", "status");
        }
         else {
            /* checking if the user has filled the profile form */
            $query = "
                SELECT uid FROM xcos_external_review_profile
                WHERE uid = :uid
            ";
            $result = db_query($query, array(':uid'=> $user->uid));
            $num_rows = $result->rowCount();
            if(!$num_rows) {
                drupal_set_message("Please fill  the profile form before you can attend the test.", "error");
                drupal_goto("xcos-tbc-external-review/profile");
            }
            
            /* showing the test details before starting the test */
            $page_content .= "<p>You are about to give a test based on which you will be selected as a reviewer for the Xcos Textbook Companions.</p>";
            $page_content .= "<ul>";
                $page_content .= "<li>The test consists of 5 code review questions that are mandatory.</li>";
                $page_content .= "<li>Test duration is 25 minutes, after which the test will automatically close.</li>";
            $page_content .= "</ul>";
            $page_content .= "<u><b>" . l("Click here", "xcos-tbc-external-review/eligibility-test/question/1") . "</b></u>";
            $page_content .= " to start the test.";
            // $page_content .= "<strong>The test has been suspended for today due to technical reasons.</strong><br>";
            // $page_content .= "<strong>Please visit this page tomorrow.</strong>";
        }
        return $page_content;
    }

    function xcos_tbc_external_review_manage_tests_all($action="", $user_id=0){
        /* 
            end the tests that did not close properly 
            i.e set status=2 before loading this page in the future\
            if the time has expired.
        */
        $page_content = "";
        if($action == "view" && $user_id) {
            /* fetching the correct answers*/
            $query = "
                SELECT * FROM xcos_external_review_eligibility_timings
                WHERE uid = :uid
            ";
            $result = db_query($query, array(':uid'=> $user_id));
            if($result->rowCount()) {
                /* fetching correct answers */
                $query = "
                    SELECT id, ncf, eit, sua, sfu, aci, auu, teo, mit, agl
                    FROM xcos_external_review_eligibility_questions
                ";
                $result = db_query($query);
                $correct_answers = array();
                while($row = $result->fetchObject()) {
                    $correct_answers[$row->id] = $row;
                }
                
                /* fetching user answers */
                $query = "
                    SELECT * FROM xcos_external_review_eligibility_answers
                    WHERE uid = :uid
                ";
                $result = db_query($query, array(':uid'=> $user_id));
                $user_answers = array();
                while($row = $result->fetchObject()) {
                    $user_answers[$row->question_id] = $row;
                }
                
                /* comparing the results */
                $scores = array();
                foreach($user_answers as $user_answer) {
                    $scores [$user_answer->question_id] = 0;
                    ($user_answer->ncf == $correct_answers[$user_answer->question_id]->ncf)?$scores[$user_answer->question_id]++:Null;
                    ($user_answer->eit == $correct_answers[$user_answer->question_id]->eit)?$scores[$user_answer->question_id]++:Null;
                    ($user_answer->sua == $correct_answers[$user_answer->question_id]->sua)?$scores[$user_answer->question_id]++:Null;
                    ($user_answer->sfu == $correct_answers[$user_answer->question_id]->sfu)?$scores[$user_answer->question_id]++:Null;
                    ($user_answer->aci == $correct_answers[$user_answer->question_id]->aci)?$scores[$user_answer->question_id]++:Null;
                    ($user_answer->auu == $correct_answers[$user_answer->question_id]->auu)?$scores[$user_answer->question_id]++:Null;
                    ($user_answer->teo == $correct_answers[$user_answer->question_id]->teo)?$scores[$user_answer->question_id]++:Null;
                    ($user_answer->mit == $correct_answers[$user_answer->question_id]->mit)?$scores[$user_answer->question_id]++:Null;
                    ($user_answer->agl == $correct_answers[$user_answer->question_id]->agl)?$scores[$user_answer->question_id]++:Null;
                }
                $page_content .= "<hr><h1><u>User Score</u></h1>";
                $total_score = 0;
                foreach($scores as $key => $value) {
                    $page_content .= "Question {$key} score = <strong>{$value}</strong> out of 9<br>";
                    $total_score += $value;
                }
                $page_content .= "<h1>Total Score = {$total_score} out of 45";
                /* fetching user data for display */
                $query = "
                    SELECT * FROM xcos_external_review_profile erd
                    LEFT JOIN users usr ON usr.uid = erd.uid
                    WHERE erd.uid = :uid
                ";
                $result = db_query($query, array(':uid' => $user_id));
                $row = $result->fetchAssoc();
                $fields = array(
                    "first_name", "last_name", "designation", "university",
                    "phone", "email", "alt_email", "address",
                    "description"
                );
                $page_content .= "<hr><h1><u>User Profile</u></h1>";
                $page_content .= "<table border=1 cellspacing=1 cellpadding=1>";
                foreach($fields as $field) {
                    $page_content .= "<tr>";
                        $page_content .= "<td>{$field}</td>";
                        $page_content .= "<td>{$row[$field]}</td>";
                    $page_content .= "</tr>";
                }
                $page_content .= "</table>";
            }
        }elseif($action == "reset" && $user_id){       
        $query = db_delete('xcos_external_review_eligibility_timings');           
            $query->condition('uid', $user_id);            
            $result = $query->execute();
            
        $query = "
        UPDATE xcos_external_review_eligibility_answers
                SET reset = 1
                WHERE uid = {:uid}
       ";
       $result = db_query($query, array(':uid'=> $user_id));
      
            drupal_set_message("Test Reset");
            drupal_goto("xcos-tbc-external-review/manage-tests");
        
       
        }
        
         else {
            $query = "
                SELECT * FROM xcos_external_review_eligibility_timings tim
                LEFT JOIN users usr ON usr.uid = tim.uid
                WHERE tim.status = 2
                ";
            $result = db_query($query);
            
            $headers = array(
                "User", "Date Attended",
                "Action"
            );
            $rows = array();
            while($row = $result->fetchObject()) {
                $item = array(
                    $row->name,
                    $row->start_date,
                    l("View", "xcos-tbc-external-review/manage-tests/view/{$row->uid}") ." | ". l("Reset", "xcos-tbc-external-review/manage-tests/reset/{$row->uid}")
                );
                array_push($rows, $item);
            }
            //$page_content .= theme("table", $headers, $rows);
            $page_content .= theme('table', array('header' => $headers, 'rows' => $rows ));
        }
        return $page_content;
    }
