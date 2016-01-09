#include"color_detection.hpp"
#include<cstdio>
#include<unistd.h>

int main(){
    
    char shotcmd[200];
    system("date");
    for(int i=0;i<10;i++){
        sprintf(shotcmd,"fswebcam -r 1280x720 -S 5 origin/%03d.jpg",i);
        system(shotcmd);
        usleep(500000);
    }
    
    char in_filename[250];
    char out_filename[2][250];
    
    
    FILE *logfp;
    char log_file_name[250];
    logfp = fopen("green_detection_log/log_jpg.txt","w");
    
    for(int i=0;i<10;++i){
        
        
        printf("----------------------------------\n");
        
        sprintf(in_filename,"origin/%03d.jpg",i);
        sprintf(out_filename[0],"phase1/%03d.jpg",i);
        sprintf(out_filename[1],"phase2/%03d.jpg",i);
        
        printf("%s\n",in_filename);
        find_color_mass fcm;
        if(-1==fcm.read_jpeg_image(in_filename)){
            continue;
        }
        fcm.init_variation();
        fcm.convert_rgb_to_hsv();
        hsv_cls btm_hsv;
    
        btm_hsv.h=70;
        btm_hsv.s=0;
        btm_hsv.v=0;
        hsv_cls tp_hsv;
        tp_hsv.h=165;
        tp_hsv.s=255;
        tp_hsv.v=255;
        hsv_cls v_hsv;
        v_hsv.h=300;
        v_hsv.s=255;
        v_hsv.v=255;
        
        float percent_of_green =100*fcm.find_hsv_mass(btm_hsv,tp_hsv,v_hsv);
        printf("parsent of green:%f%%\n",percent_of_green);
        fcm.convert_hsv_to_rgb();
        fcm.write_jpeg_image(out_filename[0]);
        float after_percent_of_green =100*fcm.equilibrium_filter();
        printf("after parsent of green:%f%%\n",after_percent_of_green);
        fcm.write_jpeg_image(out_filename[1]);
        fcm.destroy_all();
        
        fprintf(logfp,"about %03d.jpg\n",i);
        fprintf(logfp,"1:parsent of green:%f%%\n",percent_of_green);
        fprintf(logfp,"2:after parsent of equilibrium_filter:%f%%\n",after_percent_of_green);
        fprintf(logfp,"----------------------------------\n");
    }
    fclose(logfp);
    system("date");
    
}