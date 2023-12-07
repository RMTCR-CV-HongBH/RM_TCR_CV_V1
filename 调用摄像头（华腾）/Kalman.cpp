#include"Kalman.h"
Kalman::Kalman()//���캯��
{
    m_dQ = 0.0;
    m_dR = 0.0;
    // ��ʼ��������С
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

    // ��ʼ��ϵͳ����
    for (int i = 0; i < N; ++i)
    {
        m_vecSysNoise[i] = sqrt(m_dQ) * frand();
    }

    // ��ʼ���۲�����
    for (int i = 0; i < N; ++i)
    {
        m_vecObserNoise[i] = sqrt(m_dR) * frand();
    }

    // Э�����ֵ
    m_vecCov[1] = 0.01;
}

void Kalman::getData(vector<float>& vecReal,
    vector<float>& vecObserver,
    vector<float>& vecFilter)
{
    float dXPre = 0.0;   // һ��Ԥ��ֵ
    float dPpre = 0.0;   // Э����һ��Ԥ��
    float Kg = 0.0;      // �˲�����

    for (int i = 1; i < N; ++i)
    {
        m_vecReal[i] = m_vecReal[i - 1] + m_vecSysNoise[i - 1]; // ��ʵ�¶Ȳ����仯
        m_vecObser[i] = m_vecReal[i] + m_vecObserNoise[i];  // �۲�ֵ�����仯
        // �����岽ΪKalman���Ĳ���
        dXPre = m_vecKF[i - 1]; // һ��Ԥ��
        dPpre = m_vecCov[i - 1] + m_dQ;  // Э����һ��Ԥ��
        Kg = dPpre / (dPpre + m_dR);   // ��������
        m_vecKF[i] = dXPre + Kg * (m_vecObser[i] - dXPre); // ״̬����
        m_vecCov[i] = (1 - Kg) * dPpre;  // Э�������
    }
    // ������
    vecReal = m_vecReal;
    vecObserver = m_vecObser;
    vecFilter = m_vecKF;
    return;
}

void Kalman::displayError()
{
    float ObError = 0.0;   // �۲����
    float KfError = 0.0;   // �������˲����

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
    float a = 2 * ((rand() / (float)RAND_MAX) - 0.5);//�������
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