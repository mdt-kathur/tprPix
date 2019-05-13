/*
 * ========================= DirAxes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.12
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   InputINS 中 表达 方向的 数据
 *   当数据源自 joystick 时，
 *      值直接为 axis_0, axis_1
 *   当数据源自 键盘
 *      值被设置为 (-1.0, 0.0, 1.0) 三种之一
 * ----------------------------
 */
#ifndef _TPR_DIR_AXES_H_
#define _TPR_DIR_AXES_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

//-------------------- C --------------------//
#include <cassert>
#include <cmath>


class DirAxes{
public:
    DirAxes() = default;
    DirAxes( float _x, float _y ):
        x(_x),
        y(_y)
        {
            assert( (_x>=-1.0) && (_x<=1.0) &&
                    (_y>=-1.0) && (_y<=1.0) );
            this->consider_threshold_x();
            this->consider_threshold_y();
        }

    inline void clear_all(){
        this->x = 0.0;
        this->y = 0.0;
    }

    inline void set( float _x, float _y ){
        assert( (_x>=-1.0) && (_x<=1.0) &&
                (_y>=-1.0) && (_y<=1.0) );
        this->x = _x;
        this->y = _y;
        this->consider_threshold_x();
        this->consider_threshold_y();
    }

    //-- 仅用于 InputINS --
    inline void set_x( float _x ){
        assert( (_x>=-1.0) && (_x<=1.0) );
        this->x = _x;
        this->consider_threshold_x();
    }
    inline void set_y( float _y ){
        assert( (_y>=-1.0) && (_y<=1.0) );
        this->y = _y;
        this->consider_threshold_y();
    }

    inline const float &get_x() const {
        return this->x;
    }
    inline const float &get_y() const {
        return this->y;
    }

    //-- 以 float-mpos 为单位 --
    inline const glm::vec2 to_fmpos() const {
        return glm::vec2{ this->x, this->y };
    }


    inline bool is_zero() const {
        return ( (this->x==0.0) && (this->y==0.0) );
    }


    //- 目标参数 是否符合 数值范围要求 --
    static bool is_effectVal( float _x, float _y ){
        bool ret = false;
        if( (_x>=-1.0) && (_x<=1.0) && 
            (_y>=-1.0) && (_y<=1.0) ){
            if( ((_x >= DirAxes::threshold) || (_x <= -DirAxes::threshold)) ||
                ((_y >= DirAxes::threshold) || (_y <= -DirAxes::threshold)) ){
                ret = true;
            }
        }
        return ret;
    }


    //-- 计算单位向量，并调整 xy值 --
    //   确保 朝任何方向的最大速度 是一致的
    inline void limit_vals(){
        if( this->is_zero() ){
            return;
        }
        glm::vec2 normal = glm::normalize( glm::vec2{ this->x, this->y } );
        if( (abs(normal.x) < abs(this->x)) || (abs(normal.y) < abs(this->y))  ){
            this->x = normal.x;
            this->y = normal.y;
        }
    }



    //===== static =====//
    static constexpr float threshold = 0.005; //- 阈值，[-0.01, 0.01] 区间的信号不识别

private:

    //-- 将 阈值内的 微小 波动 清除 --
    inline void consider_threshold_x(){
        if( (this->x >= -DirAxes::threshold) && 
            (this->x <=  DirAxes::threshold)  ){
            this->x = 0.0;
        }
    }
    inline void consider_threshold_y(){
        if( (this->y >= -DirAxes::threshold) && 
            (this->y <=  DirAxes::threshold)  ){
            this->y = 0.0;
        }
    }

    //===== vals =====//
    float x {0.0}; //- [-1.0, 1.0]
    float y {0.0}; //- [-1.0, 1.0]
};






#endif 
