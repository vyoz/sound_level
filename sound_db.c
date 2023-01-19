#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//编译命令：
//   gcc -o sound_db sound_db.c -lm
//用法：
//   ./sound_db <file-wav> [<file-result-txt>]
//例子：
//   ./sound_db audio.wav audio.txt
//结果保留在文件: audio.txt

//每帧长度1600个采样点，0.2秒
#define FRAME_LEN 1600

double calRMS(short *samples, int num_samples)
{
    double sum_squares = 0;
    double mean_squares;
    double rms = 0.0;

    // Calculate the sum of the squares of the samples
    for (int i = 0; i < num_samples; i++) {
        sum_squares += samples[i] * samples[i];
    }

    // Divide the sum of the squares by the number of samples
    mean_squares = sum_squares / num_samples;

    // Take the square root of the mean of the squares
    rms = sqrt(mean_squares);

  return rms;
}

int main(int argc, char *argv[])
{
    FILE *fp_wav = NULL;
    FILE *fp_txt = NULL;
    short samples[FRAME_LEN];
    int num_samples = FRAME_LEN;
    double rms;
    int len;
    int n;
    double db;
    const char *file_wav = NULL;
    char file_txt[256];
    double max_db = 0.0;
    double sum_db = 0.0;
    double avg_db = 0.0;
    int max_db_i = 0;

    if( argc != 2 && argc != 3 )
    {
      fprintf(stderr, "Usage:\n");
      fprintf(stderr, "\t%s <file-wav> [<file-txt>]\n", argv[0]);
      return -1;
    }

    file_wav = argv[1];
    if( argc == 3 )
    {
      strcpy(file_txt, argv[2]);
    }else{
      sprintf(file_txt, "%s.txt", argv[1]);
    }
    

    // Open the wave file
    fp_wav = fopen(file_wav, "rb");
    if (fp_wav == NULL) {
        printf("Error opening file\n");
        return -1;
    }

    fp_txt = fopen(file_txt, "wt");
    if (fp_txt == NULL) {
        printf("Error writing file:%s\n", file_txt);
        return -1;
    }

    // 跳过文件头
    fseek(fp_wav, 40, SEEK_SET);
   
    // Read the samples from the file
    fseek(fp_wav, 44, SEEK_SET);
    n = 0;
    do{
      memset(samples, 0, sizeof(short)*FRAME_LEN);      
      len = fread(samples, sizeof(short), num_samples, fp_wav);
      if( len <= 0 )
      {
        break;
      }        

      n++;
      rms = calRMS(samples, num_samples);
      db = 20 * log10(rms);
      if( db > max_db )
      {
        max_db = db;
        max_db_i = n;
      }
        
      sum_db += db;
      printf("(%d) %f\n", n, db);
      fprintf(fp_txt, "%d %f\n", n, db);
    }while(1);

    avg_db = sum_db / n;
    printf("Max-DB(frame=%d)=%f Avg-DB=%f\n", max_db_i, max_db, avg_db);
    fprintf(fp_txt, "Max-DB(frame=%d)=%f Avg-DB=%f\n", max_db_i, max_db, avg_db);

    // Close the file and free the memory
    fclose(fp_wav);
    fclose(fp_txt);
    

    fprintf(stderr, "result saved in %s\n", file_txt);

    return 0;
}
