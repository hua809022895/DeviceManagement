import numpy as np
import math as mt
import socket
import struct
import os
import shutil
import cv2
from ctypes import *

PI = 3.1415926


'''
已知旋转角度求旋转矩阵函数
'''
def RMaix(pitch,yaw,roll):
    pitch_y = pitch;
    yaw_z = yaw;
    roll_x = roll;
    #东北天坐标系中北（X）东（Y）地（Z）
    #绕X轴旋转俯仰角 对应的矩阵Rx
    Rx = np.array([[1, 0, 0],[0,mt.cos(roll_x), mt.sin(roll_x),],[0, -mt.sin(roll_x), mt.cos(roll_x)]])
    #在右手系中绕Y轴旋转 对应的矩阵Ry
    Ry = np.array([[mt.cos(pitch_y),0,-mt.sin(pitch_y)],[0,1,0],[mt.sin(pitch_y),0,mt.cos(pitch_y)]])
    #在右手系中绕Z轴旋转 对应的矩阵Rz
    Rz = np.array([[mt.cos(yaw_z),mt.sin(yaw_z),0],[-mt.sin(yaw_z),mt.cos(yaw_z),0],[0,0,1]])
    return np.dot(np.dot(Rx,Ry),Rz)
'''
计算相机的深度值函数,默认所有物体高度值都为0
'''
def Camera_depth(CameraUavRMatrix_inv,cameraMatrix_inv,imagePoint,zConst,tvec):    
    tempMat_ini = np.dot(CameraUavRMatrix_inv,cameraMatrix_inv)
    tempMat=np.dot(tempMat_ini,imagePoint)
    tempMat2 = np.dot(CameraUavRMatrix_inv,tvec)
    s = zConst - tempMat2[2, 0];
    s /= tempMat[2, 0];
    return s
'''
从距离转为经纬度，首先通过距离、航向、经纬度计算出该点经纬度值,然后通过旋转矩阵进行旋转
'''
def DisConvertLonAndLat(UavData,wDisPoint):
    #赋初值协议中yaw pitch roll以及航向角都是弧度
    HeadingAngle_theta = -UavData[6,0]
    uav_longitude = UavData[1,0]
    uav_latitude = UavData[0,0]
    uav_heigth = UavData[2,0]
    pitch = UavData[4,0]
    yaw = UavData[3,0]
    roll = UavData[5,0]

    #求旋转矩阵
    lat_lon_RMaix = RMaix(pitch,yaw,roll)
    lat_lon_RMaix_inv = np.linalg.inv(lat_lon_RMaix)

    #目标点的坐标绕中心点旋转后坐标如下：  
    #delta_longitude = (wDisPoint[1,0]*mt.cos(HeadingAngle_theta)-wDisPoint[0,0]*mt.sin(HeadingAngle_theta))/(111320*mt.cos(uav_latitude*3.14/180))
    delta_longitude = wDisPoint[1,0]/(111320*mt.cos(uav_latitude*PI/180))
    #变化的纬度
    #delta_latitude = (wDisPoint[0,0]*mt.cos(HeadingAngle_theta)+wDisPoint[1,0]*mt.sin(HeadingAngle_theta))/110540
    delta_latitude = wDisPoint[0,0]/(110540)

    #变化后的经度
    target_lon = uav_longitude+delta_longitude

    #变化后的纬度
    target_lat = uav_latitude+delta_latitude
    target_height = 0
    return [target_lat,target_lon]
    
#根据无人机的数据及目标在图像中的像素数据求目标的实际位置    
def PixWorld(UavData,PictureData):   
    
    '''
    定义相机内参矩阵
    定义相机的内参矩阵,并赋值f为焦距，单位mm,dx为每个像素在横轴x的尺寸单位是mm/像素，dy为每个像素在横轴y的尺寸
    根据标定数据可算出相机视角为62.45、37.69度
    fx = 1580.81637
    fy = 1582.1537
    cx = 970.64286
    cy = 555.57635
    
    #海康大相机标定数据
    fx = 2338.14087;
    fy = 2334.89369;
    cx = 2048.81869;
    cy = 1500.13787;
    disCoeffs0 = 0.10591;
    disCoeffs1 = -0.19833;
    disCoeffs2 = 0.00098;
    disCoeffs3 = 0.00171;
    disCoeffs4 = 0;
    '''
   
    #海康小相机标定数据
    fx = 4160.99320;
    fy = 4240.68520;
    cx = 1810.08011;
    cy = 1451.62771;
    w = 4032
    h = 3036

    cameraMatrix = np.array([[fx,0,cx],[0,fy,cy],[0,0,1]])
  
    #定义相机的畸变参数并赋值，不同的相机需要提前标定，可用matlab标定工具箱或opencv均可
    disCoeffs0 = -0.44622 ;
    disCoeffs1 = 0.38641;
    disCoeffs2 = 0.00675;
    disCoeffs3 = 0.00608;
    disCoeffs4 = 0;
    disCoeffs = np.array([disCoeffs0,disCoeffs1,disCoeffs2,disCoeffs3,disCoeffs4])
    #将畸变引进去之后新的相机内参矩阵
    #cameraMatrix,roi = cv2.getOptimalNewCameraMatrix(cameraMatrix,disCoeffs,(w,h),1,(w,h))
    #print("纠正后相机内参==",cameraMatrix)
    
    
    #定义相机与无人机坐标系的平移向量,即相机坐标系与无人机世界坐标系原点之间的平移向量
    tvec = np.array([[0],[0],[-UavData[2,0]]])
    #固定旋转矩阵为CameraUavRMatrix
    #定义相机与无人机坐标系的旋转矩阵,该矩阵跟安装位置有关系，是固定的
   
    camera_pitch_y = 0;
    camera_yaw_z = 90*PI/180;
    camera_roll_x = 0;
    CameraUavRMatrix = RMaix(camera_pitch_y,camera_yaw_z,camera_roll_x)
    
    #定义无人机的姿态矩阵_0922新增
    pitch = UavData[4,0]
    yaw = UavData[3,0]
    roll = UavData[5,0]
    UavRMatrix = RMaix(pitch,yaw,roll)
    UavRMatrix_inv = np.linalg.inv(UavRMatrix)   
    
    #求相机的深度值Zc即s,地面物体的实际高度zConst = 0，物体的实际高度不一定，该值不一定，此算法中默认物体高度为0
    zConst = 0
    CameraUavRMatrix_inv = np.linalg.inv(CameraUavRMatrix)
    cameraMatrix_inv = np.linalg.inv(cameraMatrix)
    imagePoint = np.array([[2016],[1516],[1]])
    s = Camera_depth(CameraUavRMatrix_inv,cameraMatrix_inv,imagePoint,zConst,tvec)
    
    #根据已知参数求大地坐标转换数据，像素对应的离图像中心的距离为wDisPoint
    my_picture_pix = PictureData
    wDisPoint_ini = np.dot(cameraMatrix_inv,my_picture_pix)*s
    wDisPoint = np.dot(CameraUavRMatrix_inv,wDisPoint_ini)
    
    #根据已知参数求无人机坐标转换数据，像素对应的离图像中心的距离为wDisPoint
    world_position = np.dot(UavRMatrix_inv,wDisPoint)
    #根据旋转角度求图像中像素点距离无人机各个方向的实际距离
    if pitch == 0 and roll == 0:
        world_position_x = world_position[0,0]
        world_position_y = world_position[1,0]
        world_position_z = world_position[2,0]

    else:        
        world_position_x = world_position[0,0]*UavData[2,0]/(world_position[2,0])
        world_position_y = world_position[1,0]*UavData[2,0]/(world_position[2,0])
        world_position_z = world_position[2,0]  
    
        
    #像素对应的到无人机的距离world_position_end      
    world_position_end = np.array([[world_position_x],[world_position_y],[world_position_z]])
   # print("world_position_end=",world_position_end)     

    '''
    #根据离像素离相机中心点的距离Dis及无人机的经纬度航向角
    及pitch、yaw、rol三个姿态角解算出目标位置的经纬度
    '''
    #假设航向角HeadingAngle度
    #无人机的经纬度如下
    target=DisConvertLonAndLat(UavData,world_position_end)
    return target
#-----------------------------------------------------------------------------------------------------------------------------    
#---------------------------------------------------------------------------------------------------
if(__name__=='__main__'):
    #uav数据为纬度 经度  高度 yaw pitch roll 航向角
    print("解算模块初始化了...");
    #''' 
    #UavData = np.array([[38.1356048584],[116.2952423096],[288.55],[0.090493],[-0.041629],[-0.007741],[0]])
    #PictureData = np.array([[1779],[1328],[1]])
    UavData = np.array([[38.1347198486],[116.2951507568],[287.55],[3.074225],[0.045621],[-0.000837],[0]])
    PictureData = np.array([[1754],[2735],[1]])
    print("PictureData=",PictureData)    
    target_postion=PixWorld(UavData,PictureData)
    print('target_postion=',target_postion)
    #'''














    

