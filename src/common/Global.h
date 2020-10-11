#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QMetaType>

/* 城市当天天气信息 */
typedef struct tagCityTodayWeather {
    QString cityName;       //城市名称
    QString cityId;         //城市编码
    QString date;           //当前日期
    QString updateTime;     //天气更新时间
    QString week;           //星期
    QString temperature;    //温度
    QString temperatureLow; //最低温度
    QString temperatureHigh; //最高温度
    QString weatherDesc;    //实况天气状况中文
    QString weatherId;      //实况天气ID
    QString humidity;       //湿度
    QString windDirection;  //风向
    QString windScale;      //风力
    QString aqi;            //空气质量
    QString aqiLevel;       //空气质量等级
    QString aqiTips;        //空气质量描述
    QString alarmType;      //预警种类
    QString alarmLevel;     //预警等级
    QString alarmContent;   //预警内容
} CityTodayWeather;

typedef struct tagCityFutureWeather {
    QString date;           //当前日期
    QString day;            //星期
    QString week;           //星期
    QString temperatureLow; //最低温度
    QString temperatureHigh; //最高温度
    QString weatherDesc;    //实况天气状况中文
    QString weatherId;      //实况天气ID
    QString windSpeed;      //风力
} CityFutureWeather;
Q_DECLARE_METATYPE(CityFutureWeather)

/* 城市当天生活指数 健身 洗车 穿衣 紫外线 粉尘 舒适度 感冒 晾晒*/
enum LifeIndex {fitness = 1, car_washing = 2, clothe = 3, uv = 5, dust = 7, comfort = 8, sick = 9,  hanger = 14};
/* 城市当天生活指数信息 */
typedef struct tagCityLifeIndex {
    LifeIndex   type;
    QString     level;
    QString     title;
    QString     desc;
    QString     icon;
} CityLifeIndex;
Q_DECLARE_METATYPE(CityLifeIndex)

/* 热门城市 */
typedef struct tagTopCity {
    QString cityName;
    QString DistrictName;
    QString cityId;
} TopCity;

#endif // GLOBAL_H
