#include "xiaozhi.h"
#include "xiaozhi_ai.h"
#include "ui/src/ui.h"

void xiaozhi_update()
{
    if(Xiaozhi.ask_flag == 1)
    {
        if(Xiaozhi.answer_flag == 0)
        {
            xiaozhi_loop();
        }

        if (xiaozhi_listen()) 
        {
            Xiaozhi.question_text = xiaozhi_question();
            lv_textarea_set_text(ui_TextAreaXiaozhiQuestion, Xiaozhi.question_text.c_str());
        }

        if (xiaozhi_speak()) 
        {
            Xiaozhi.answer_text = xiaozhi_answer(0);
            lv_textarea_set_text(ui_TextAreaXiaoZhiAnswer, Xiaozhi.answer_text.c_str());  
            Xiaozhi.answer_url = xiaozhi_answer(1);
            Music.play_flag = 1;
            Music.start_flag = 1;
            Xiaozhi.answer_flag = 1;  
            if(lv_obj_has_state(ui_StartMusic, LV_STATE_CHECKED))
            {
                lv_obj_clear_state(ui_StartMusic, LV_STATE_CHECKED);
            }       
        }
    }
}