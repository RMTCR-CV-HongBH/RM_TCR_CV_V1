#include"Kalman.h"
Kalman::Kalman()//构造函数
{
    m_dQ = 0.0;
    m_dR = 0.0;
    // 初始化容器大小
    m_vecSysNoise.resize(N);
    m_vecObserNoise.resize(N);
    m_vecReal.resize(N);
    m_vecObser.resize(N);
    m_vecKF.resize(N);
    m_vecCov.resize(N);
}

void Kalman::setIniVal(float dval, float dQ, float dR)
{
    m_dQ = dQ;
    m_dR = dR;
    m_vecReal[0] = dval;
    m_vecObser[0] = dval;
    m_vecKF[0] = dval;

    // 初始化系统噪声
    for (int i = 0; i < N; ++i)
    {
        m_vecSysNoise[i] = sqrt(m_dQ) * frand();
    }

    // 初始化观测噪声
    for (int i = 0; i < N; ++i)
    {
        m_vecObserNoise[i] = sqrt(m_dR) * frand();
    }

    // 协方差赋初值
    m_vecCov[1] = 0.01;
}

void Kalman::getData(vector<float>& vecReal,
    vector<float>& vecObserver,
    vector<float>& vecFilter)
{
    float dXPre = 0.0;   // 一步预测值
    float dPpre = 0.0;   // 协方差一步预测
    float Kg = 0.0;      // 滤波增益

    for (int i = 1; i < N; ++i)
    {
        m_vecReal[i] = m_vecReal[i - 1] + m_vecSysNoise[i - 1]; // 真实温度波动变化
        m_vecObser[i] = m_vecReal[i] + m_vecObserNoise[i];  // 观测值波动变化
        // 以下五步为Kalman核心步骤
        dXPre = m_vecKF[i - 1]; // 一步预测
        dPpre = m_vecCov[i - 1] + m_dQ;  // 协方差一步预测
        Kg = dPpre / (dPpre + m_dR);   // 计算增益
        m_vecKF[i] = dXPre + Kg * (m_vecObser[i] - dXPre); // 状态更新
        m_vecCov[i] = (1 - Kg) * dPpre;  // 协方差更新
    }
    // 输出结果
    vecReal = m_vecReal;
    vecObserver = m_vecObser;
    vecFilter = m_vecKF;
    return;
}

void Kalman::displayError()
{
    float ObError = 0.0;   // 观测误差
    float KfError = 0.0;   // 卡尔曼滤波误差

    for (int i = 0; i < N; ++i)
    {
        cout << " Real Value " << setprecision(5) << m_vecReal[i];
        cout << " Observer Value " << setprecision(5) << m_vecObser[i];
        cout << " Error " << fabs(m_vecObser[i] - m_vecReal[i]);
        cout << " KF Value " << setprecision(5) << m_vecKF[i];
        cout << " Error " << fabs(m_vecKF[i] - m_vecReal[i]) << "\n";
        ObError += fabs(m_vecReal[i] - m_vecObser[i]);
        KfError += fabs(m_vecReal[i] - m_vecKF[i]);
    }

    cout << "\n" << " Observer Error " << setprecision(5) << ObError << "\n";
    cout << " KalmanFilter Error " << setprecision(5) << KfError;
}



float Kalman::frand()
{
    static int seed = 0;
    int i = time(0) % 100000;
    seed += i;
    srand(seed);
    float a = 2 * ((rand() / (float)RAND_MAX) - 0.5);//随机噪声
    return a;
}

//int main()
//{
//
//    Kalman kal;
//    vector<float> vecReal;
//    vector<float> vecOb;
//    vector<float> vecKF;
//
//    kal.setIniVal(30);
//    kal.getData(vecReal, vecOb, vecKF);
//    kal.displayError();
//    return 0;
//}