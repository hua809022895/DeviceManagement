import numpy as np
import math as mt
import socket
import struct
import os
import shutil
from ctypes import *
import logging
import piture_world_v4 as piwo; 

#参数依次为图片目标点坐标，无人机纬度，无人机经度，无人机高度，yaw,pitch,roll----------------------------    
def getPostion(picX,picY,lat,lng, height,yaw,pitch,roll):  #根据图片上的目标点，无人机姿态数据，解算出经纬度值并返回
   UavData     = np.array([[lat],[lng],[height],[yaw],[pitch],[roll],[0]])
   PictureData = np.array([[picX],[picY],[1]]);   
   pos= piwo.PixWorld(UavData, PictureData);
   return str(pos[0]) +","+ str(pos[1]);
#---------------------------------------------------------------------------------------------------
if(__name__=='__main__'):
   #uav数据为纬度 经度  高度 yaw pitch roll 航向角
   print("解算模块初始化了...");
    
   #UavData    = np.array([[air.lat],[air.lng], [air.height], [air.yaw],[air.pitch], [air.roll],[0]]);
   lat=38.1355018;
   lng=116.295314;
   UavData     = np.array([[lat],[lng],[26.8],[4.23729],[0.-0.0759],[0.00522],[0]])
   PictureData = np.array([[0],[0],[1]]);   
   pos= piwo.PixWorld(UavData, PictureData);
   print('左上=', pos)
    
   PictureData = np.array([[4096],[0],[1]]);   
   pos= piwo.PixWorld(UavData, PictureData);
   print('右上=', pos)

   PictureData = np.array([[0],[3000],[1]]);   
   pos= piwo.PixWorld(UavData, PictureData);
   print('左下=', pos)

   PictureData = np.array([[4096],[3000],[1]]);   
   pos= piwo.PixWorld(UavData, PictureData);
   print('右下=', pos)









    

