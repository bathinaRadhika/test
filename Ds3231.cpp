#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#define BUFFER_SIZE 19
#include<unistd.h>
using namespace std;
#include<iostream>
class Ds3231{
 int file;
  public:
    void set_values (int);
    int bus_check() {

    if(file < 0){
      perror("failed to open the bus\n");
      return 1;
   }
   }

};


// the time is in the registers in encoded decimal form
int bcdToDec(char b) { return (b/16)*10 + (b%16); }

void Ds3231::set_values (int x) {
  file = x;
}

int main(){
    printf("Starting the DS3231 test application\n");
    Ds3231 ds_obj;
    ds_obj.set_values(open("/dev/i2c-1", O_RDWR));
    ds_obj.bus_check();


   if(ioctl(ds_obj.bus_check(), I2C_SLAVE, 0x68) < 0){
      perror("Failed to connect to the sensor\n");
      return 1;
   }
   char writeBuffer[1] = {0x00};
   if(write(ds_obj.bus_check(), writeBuffer, 1)!=1){
      perror("Failed to reset the read address\n");
      return 1;
   }
   char buf[BUFFER_SIZE];
   if(read(ds_obj.bus_check(), buf, BUFFER_SIZE)!=BUFFER_SIZE){
      perror("Failed to read in the buffer\n");
      return 1;
   }

    cout << "The RTC time is " << bcdToDec(buf[2]) << ":" <<
         bcdToDec(buf[1]) << ":"<< bcdToDec(buf[0]) << endl;
    cout << "The RTC Date is\n" << "Date: " << bcdToDec(buf[4]) << " Month: 0" << bcdToDec(buf[5]) << " Year: 0" << bcdToDec(buf[6]) << endl;
    //cout << var1 << "Some String" << var2 << endl;
   close(ds_obj.bus_check());
   return 0;
}
