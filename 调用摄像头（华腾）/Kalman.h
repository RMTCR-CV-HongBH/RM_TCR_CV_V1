#include <vector>
#include<iostream>
#include <math.h>
#include <cstdlib>
#include <iomanip>

#define N 50
using namespace std;
class Kalman
{
public:
    Kalman();
    /**
     * @brief setIniVal 设置初始值
     * @param dval 待测数据初始值
     * @param dQ   系统噪声方差(有默认值)
     * @param dR   观测噪声方差(有默认值)
     */
    void setIniVal(float dval, float dQ = 0.01, float dR = 0.25);

    /**
     * @brief getData 获取滤波数据
     * @param vecReal 真实值
     * @param vecObserver 观测值
     * @param vecFilter 滤波后的值
     */
    void getData(vector<float>& vecReal,
        vector<float>& vecObserver,
        vector<float>& vecFilter);
    // 显示滤波效果
    void displayError();

private:
    // 产生-1与1之间的随机数
    float frand();

private:
    float m_dQ;       // 系统噪声方差
    float m_dR;       // 观测噪声方差
    vector<float> m_vecSysNoise;   // 系统噪声
    vector<float> m_vecObserNoise; // 观测噪声
    vector<float> m_vecReal;   // 真实值
    vector<float> m_vecObser;  // 观测值
    vector<float> m_vecKF;     // 滤波值
    vector<float> m_vecCov;    // 协方差
};