#include"color_detection.hpp"
#include<cstdio>

int main(){
    
    char in_filename[250];
    char out_filename[2][250];
    
    
    FILE *logfp;
    char log_file_name[250];
    logfp = fopen("log_jpg.txt","w");
    
    for(int i=0;i<3;++i){
        
        
        printf("----------------------------------\n");
        
        sprintf(in_filename,"img/sample%03d.jpg",i);
        sprintf(out_filename[0],"img/phase1%03d.jpg",i);
        sprintf(out_filename[1],"img/phase2%03d.jpg",i);
        
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
        printf("parsent of color:%f%%\n",percent_of_green);
        fcm.convert_hsv_to_rgb();
        fcm.write_jpeg_image(out_filename[0]);
        float after_percent_of_green =100*fcm.equilibrium_filter();
        printf("after parsent of color:%f%%\n",after_percent_of_green);
        fcm.write_jpeg_image(out_filename[1]);
        fcm.destroy_all();
        
        fprintf(logfp,"about sample%03d.jpg\n",i);
        fprintf(logfp,"1:parsent of color:%f%%\n",percent_of_green);
        fprintf(logfp,"2:after parsent of equilibrium_filter:%f%%\n",after_percent_of_green);
        fprintf(logfp,"----------------------------------\n");
    }
    fclose(logfp);
    
}