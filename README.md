# CoCalendar
## 项目介绍
日历数据存储及转换

#### 支持的日历

- 农历(Chinese)
- 格里高利历(Gregorian)

## 环境依赖
VS2022
## 目录结构
    CoCalendar
    │  .gitignore
    │  Astronomy.h
    │  AstronomyTest.cpp
    │  AstronomyTest.h
    │  Calendar.cpp
    │  Calendar.h
    │  Calendar.sln
    │  Calendar.vcxproj
    │  Calendar.vcxproj.filters
    │  CalendarDef.cpp
    │  CalendarDef.h
    │  CalendarMath.cpp
    │  CalendarMath.h
    │  ChineseCalendar.cpp
    │  ChineseCalendar.h
    │  converter.cpp
    │  converter.h
    │  ConverterManager.cpp
    │  ConverterManager.h
    │  GregorianCalendar.cpp
    │  GregorianCalendar.h
    │  README.md
    │  test.cpp

## 版本内容更新
###### v1.0.0
1.实现基本功能

## TODO

1. 增加日历类型
2. 完善时间可用性的判断与限制
3. 实现日历转换路径的自动推导
4. 完善对时间的计算(时分秒)
5. 实现Locale

## 许可证
MIT License

## 鸣谢

农历计算来源summerinsects/ChineseCalendar
