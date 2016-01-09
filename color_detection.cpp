/*Formula_base_http://hooktail.org/computer/index.php?RGB%A4%AB%A4%E9HSV%A4%D8%A4%CE%CA%D1%B4%B9%A4%C8%C9%FC%B8%B5
               http://www.peko-step.com/tool/hsvrgb.html*/

/*this software is based in part on the work of the Independent JPEG Group
  このソフトウェアはthe Independent JPEG Groupのlibjpegを使用しています*/

#include"color_detection.hpp"

int find_color_mass::read_jpeg_image(char file_name[]){
    
    struct jpeg_error_mgr jerr;
    
    FILE *infile;
    
    int width;
    int height;
    long file_size=0;
    
    jpeg_cinfo_.err = jpeg_std_error( &jerr );
    jpeg_create_decompress( &jpeg_cinfo_ );
    
    infile = fopen(file_name, "rb" );
    if(infile==NULL){
        printf("Cannot Open %s\n",file_name);
        return -1;
    }
    
    fseek( infile, 0, SEEK_END );
    file_size = ftell(infile);
    fseek(infile, 0, SEEK_SET );
    printf("file_size:%ld\n",file_size);
    if(file_size<=0){
        fclose(infile);
        return -1;
    }
    jpeg_stdio_src( &jpeg_cinfo_, infile );
    
    jpeg_read_header( &jpeg_cinfo_, TRUE );
    
    jpeg_start_decompress( &jpeg_cinfo_ );
    
    width = jpeg_cinfo_.output_width;
    height = jpeg_cinfo_.output_height;
    
    origin_jpeg_img_ = new JSAMPROW[height];
    mask_jpeg_img_ = new JSAMPROW[height];
    for (int px_y = 0; px_y < height; px_y++ ) {
        origin_jpeg_img_[px_y] = new JSAMPLE[3*width];
        mask_jpeg_img_[px_y] = new JSAMPLE[3*width];
        for(int px_x=0;px_x<width;px_x++){
            origin_jpeg_img_[px_y][px_y]=0;
            mask_jpeg_img_[px_y][px_y]=0;
        }
    }
    
    while( jpeg_cinfo_.output_scanline < jpeg_cinfo_.output_height ) {
        jpeg_read_scanlines( &jpeg_cinfo_,
                            origin_jpeg_img_ + jpeg_cinfo_.output_scanline,
                            jpeg_cinfo_.output_height - jpeg_cinfo_.output_scanline
                            );
    }
    
    jpeg_finish_decompress(&jpeg_cinfo_);
    
    jpeg_destroy_decompress(&jpeg_cinfo_);
    
    fclose(infile);
    return 0;
}

void find_color_mass::init_variation(/*int img_width,int img_height*/){
    int img_width =jpeg_cinfo_.output_width;
    int img_height = jpeg_cinfo_.output_height;
    img_.height = img_height;
    img_.width = img_width;
    img_.r_hsv = new hsv_cls*[img_.height];
    img_.hsv = new hsv_cls*[img_.height];
    for(int px_y=0;px_y<img_.height;px_y++){
        img_.r_hsv[px_y] = new hsv_cls[img_.width];
        img_.hsv[px_y] = new hsv_cls[img_.width];
    }
}

void find_color_mass::convert_rgb_to_hsv(){
    float max_rgb,min_rgb,max_rgb_num;
    int rgb_cal_num[2];
    
    for(int px_y=0;px_y<img_.height;px_y++){
        for(int px_x=0;px_x<img_.width;px_x++){
            
            img_.r_hsv[px_y][px_x].h=0;
            img_.r_hsv[px_y][px_x].s=0;
            img_.r_hsv[px_y][px_x].v=0;
            
            max_rgb=origin_jpeg_img_[px_y][px_x*3+0];
            min_rgb=origin_jpeg_img_[px_y][px_x*3+0];
            max_rgb_num=0;
            rgb_cal_num[0]=origin_jpeg_img_[px_y][px_x*3+2];
            rgb_cal_num[1]=origin_jpeg_img_[px_y][px_x*3+1];
            
            if(max_rgb<origin_jpeg_img_[px_y][px_x*3+1]){
                max_rgb=origin_jpeg_img_[px_y][px_x*3+1];
                max_rgb_num=2;
                rgb_cal_num[0]=origin_jpeg_img_[px_y][px_x*3+0];
                rgb_cal_num[1]=origin_jpeg_img_[px_y][px_x*3+2];
            }
            
            if(max_rgb<origin_jpeg_img_[px_y][px_x*3+2]){
                max_rgb=origin_jpeg_img_[px_y][px_x*3+2];
                max_rgb_num=4;
                rgb_cal_num[0]=origin_jpeg_img_[px_y][px_x*3+1];
                rgb_cal_num[1]=origin_jpeg_img_[px_y][px_x*3+0];
            }
            
            if(min_rgb>origin_jpeg_img_[px_y][px_x*3+1]){
                min_rgb=origin_jpeg_img_[px_y][px_x*3+1];
            }
            if(min_rgb>origin_jpeg_img_[px_y][px_x*3+2]){
                min_rgb=origin_jpeg_img_[px_y][px_x*3+2];
            }
            if(max_rgb-min_rgb!=0.0f){
                img_.hsv[px_y][px_x].v=max_rgb;
                img_.hsv[px_y][px_x].s=255*(max_rgb-min_rgb)/max_rgb;
                img_.hsv[px_y][px_x].h=60*(max_rgb_num+((rgb_cal_num[1]-rgb_cal_num[0])/(max_rgb-min_rgb)));
                //printf("%d\n",60*(max_rgb_num+((rgb_cal_num[1]-rgb_cal_num[0])/(max_rgb-min_rgb))));
            }else{
                img_.hsv[px_y][px_x].v=max_rgb;
                img_.hsv[px_y][px_x].s=0;
                img_.hsv[px_y][px_x].h=0;
            }
            if(img_.hsv[px_y][px_x].h>=360){
                img_.hsv[px_y][px_x].h-=360;
            }
            if(img_.hsv[px_y][px_x].h<0){
                img_.hsv[px_y][px_x].h+=360;
            }
            /*if(img_.hsv[px_y][px_x].s>255||img_.hsv[px_y][px_x].s<0){
                printf("s:%d",img_.hsv[px_y][px_x].s);
            }
            if(img_.hsv[px_y][px_x].v>255||img_.hsv[px_y][px_x].v<0){
                printf("v:%d",img_.hsv[px_y][px_x].v);
            }*/
        }
    }
}


float find_color_mass::find_hsv_mass(hsv_cls bottom_hsv,hsv_cls top_hsv,hsv_cls variation_hsv){
    int mass_counter=0;
    for(int px_y=0;px_y<img_.height;px_y++){
        for(int px_x=0;px_x<img_.width;px_x++){
            if(bottom_hsv.v<=img_.hsv[px_y][px_x].v && img_.hsv[px_y][px_x].v<=top_hsv.v){
                if(bottom_hsv.s<=img_.hsv[px_y][px_x].s && img_.hsv[px_y][px_x].s<=top_hsv.s){
                    if(top_hsv.h<=bottom_hsv.h){
                        if(img_.hsv[px_y][px_x].h<=top_hsv.h||img_.hsv[px_y][px_x].h>=bottom_hsv.h){
                            img_.r_hsv[px_y][px_x].h=variation_hsv.h;//+=variation_hsv.h;
                            img_.r_hsv[px_y][px_x].s=variation_hsv.s;//+=variation_hsv.s;
                            img_.r_hsv[px_y][px_x].v=variation_hsv.v;//+=variation_hsv.v;
                            ++mass_counter;
                        }
                    }else{
                        if(img_.hsv[px_y][px_x].h<=top_hsv.h&&img_.hsv[px_y][px_x].h>=bottom_hsv.h){
                            img_.r_hsv[px_y][px_x].h=variation_hsv.h;//+=variation_hsv.h;
                            img_.r_hsv[px_y][px_x].s=variation_hsv.s;//+=variation_hsv.s;
                            img_.r_hsv[px_y][px_x].v=variation_hsv.v;//+=variation_hsv.v;
                            ++mass_counter;
                        }
                    }
            
                    if(img_.r_hsv[px_y][px_x].h>=360){
                        img_.r_hsv[px_y][px_x].h-=360;
                    }
                    if(img_.r_hsv[px_y][px_x].h<0){
                        img_.r_hsv[px_y][px_x].h+=360;
                    }
                }
            }
        }
    }
    printf("pix:%d\nheight*width%d\n",mass_counter,img_.height*img_.width);
    if(img_.height*img_.width==0.0f){
        printf("zero return\n");
        return 0.0;
    }
    return (float)mass_counter/(float)(img_.height*img_.width);
}

void find_color_mass::convert_hsv_to_rgb(){
    for(int px_y=0;px_y<img_.height;px_y++){
        for(int px_x=0;px_x<img_.width;px_x++){
            if(img_.r_hsv[px_y][px_x].s==0){
                mask_jpeg_img_[px_y][px_x*3+0] = 0;
                mask_jpeg_img_[px_y][px_x*3+1] = 0;
                mask_jpeg_img_[px_y][px_x*3+2] = 0;
            }else{
                int max_hsv = img_.r_hsv[px_y][px_x].v;
                int min_hsv = max_hsv - (img_.r_hsv[px_y][px_x].s/255)*max_hsv;
                int h_sub=img_.r_hsv[px_y][px_x].h/60;
                
                switch(h_sub){
                    case 0:
                        mask_jpeg_img_[px_y][px_x*3+0] = max_hsv;
                        mask_jpeg_img_[px_y][px_x*3+1] = (img_.r_hsv[px_y][px_x].h/60)*(max_hsv-min_hsv)+min_hsv;
                        mask_jpeg_img_[px_y][px_x*3+2] = min_hsv;
                        break;
                    case 1:
                        mask_jpeg_img_[px_y][px_x*3+0] = ((120-img_.r_hsv[px_y][px_x].h)/60)*(max_hsv-min_hsv)+min_hsv;
                        mask_jpeg_img_[px_y][px_x*3+1] = max_hsv;
                        mask_jpeg_img_[px_y][px_x*3+2] = min_hsv;
                        break;
                    case 2:
                        mask_jpeg_img_[px_y][px_x*3+0] = min_hsv;
                        mask_jpeg_img_[px_y][px_x*3+1] = max_hsv;
                        mask_jpeg_img_[px_y][px_x*3+2] = ((img_.r_hsv[px_y][px_x].h-120)/60)*(max_hsv-min_hsv)+min_hsv;
                        break;
                    case 3:
                        mask_jpeg_img_[px_y][px_x*3+0] = min_hsv;
                        mask_jpeg_img_[px_y][px_x*3+1] = ((240-img_.r_hsv[px_y][px_x].h)/60)*(max_hsv-min_hsv)+min_hsv;
                        mask_jpeg_img_[px_y][px_x*3+2] = max_hsv;
                        break;
                    case 4:
                        mask_jpeg_img_[px_y][px_x*3+0] = ((img_.r_hsv[px_y][px_x].h-240)/60)*(max_hsv-min_hsv)+min_hsv;
                        mask_jpeg_img_[px_y][px_x*3+1] = min_hsv;
                        mask_jpeg_img_[px_y][px_x*3+2] = max_hsv;
                        break;
                    case 5:
                        mask_jpeg_img_[px_y][px_x*3+0] = max_hsv;
                        mask_jpeg_img_[px_y][px_x*3+1] = min_hsv;
                        mask_jpeg_img_[px_y][px_x*3+2] = ((360-img_.r_hsv[px_y][px_x].h)/60)*(max_hsv-min_hsv)+min_hsv;
                        break;
                }
            }
        }
    }
}

float find_color_mass::equilibrium_filter(){
    int** one_is_black = new int*[img_.height];//1:black,-1:other(also:white)
    int mass_counter=0;
    
    for(int px_y=0;px_y<img_.height;px_y++){
        one_is_black[px_y]=new int[img_.width];
        for(int px_x=0;px_x<img_.width;px_x++){
            one_is_black[px_y][px_x]=0;
        }
    }
    
    for(int px_y=0;px_y<img_.height;px_y++){
        for(int px_x=0;px_x<img_.width;px_x++){
            
            int black_counter=0,white_counter=0;
            for(int sub_px_y=px_y-1;sub_px_y<=px_y+1;sub_px_y++){
                if(sub_px_y<0||sub_px_y>=img_.height){
                    continue;
                }
                for(int sub_px_x=px_x-1;sub_px_x<=px_x+1;sub_px_x++){
                    if(sub_px_x<0||sub_px_x>=img_.width){
                        continue;
                    }
                    if(one_is_black[sub_px_y][sub_px_x]==0){
                        int loop_counter=0;
                        for(int i=0;i<3;i++){
                            if(mask_jpeg_img_[sub_px_y][sub_px_x*3+i]==0){
                                ++loop_counter;
                            }
                        }
                        if(loop_counter>2){
                            ++black_counter;
                            one_is_black[sub_px_y][sub_px_x]=1;
                        }else{
                            ++white_counter;
                            one_is_black[sub_px_y][sub_px_x]=-1;
                        }
                    }else{
                        if(one_is_black[sub_px_y][sub_px_x]==1){
                            ++black_counter;
                        }else{
                            ++white_counter;
                        }
                    }
                    if(black_counter>5||white_counter>5){
                        break;
                    }
                }
                if(black_counter>5||white_counter>5){
                    break;
                }
            }
            if(black_counter>white_counter){
                mask_jpeg_img_[px_y][px_x*3+0]=0;
                mask_jpeg_img_[px_y][px_x*3+1]=0;
                mask_jpeg_img_[px_y][px_x*3+2]=0;
            }else{
                mask_jpeg_img_[px_y][px_x*3+0]=255;
                mask_jpeg_img_[px_y][px_x*3+1]=0;
                mask_jpeg_img_[px_y][px_x*3+2]=255;
                ++mass_counter;
            }
        }
    }
    
    for (int px_y = 0; px_y < img_.height; px_y++){
        free(one_is_black[px_y]);
    }
    free(one_is_black);
    
    if(img_.height*img_.width==0.0f){
        printf("zero return\n");
        return 0.0;
    }
    
    return (float)mass_counter/(float)(img_.height*img_.width);
}

void find_color_mass::write_jpeg_image(char file_name[]){
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile;
    
    cinfo.err = jpeg_std_error( &jerr );
    
    cinfo.err = jpeg_std_error( &jerr );
    jpeg_create_compress(&cinfo);
    
    jpeg_create_compress(&cinfo);
    
    outfile = fopen(file_name, "wb" );
    jpeg_stdio_dest(&cinfo,outfile);
    
    cinfo.image_width = img_.width;
    cinfo.image_height = img_.height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    
    jpeg_set_defaults( &cinfo );
    jpeg_set_quality( &cinfo, 80, TRUE );
    
    jpeg_start_compress( &cinfo, TRUE );
    
    jpeg_write_scanlines( &cinfo,mask_jpeg_img_,img_.height);
    
    jpeg_finish_compress( &cinfo );
    
    jpeg_destroy_compress( &cinfo );
    
    fclose(outfile);
}

void find_color_mass::destroy_all(){
    for(int px_y=0;px_y<img_.height;px_y++){
        delete[] img_.hsv[px_y];
        delete[] origin_jpeg_img_[px_y];
        delete[] mask_jpeg_img_[px_y];
    }
    delete[] img_.hsv;
    delete[] origin_jpeg_img_;
    delete[] mask_jpeg_img_;
}


