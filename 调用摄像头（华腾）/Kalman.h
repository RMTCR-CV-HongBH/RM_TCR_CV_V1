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
     * @brief setIniVal ���ó�ʼֵ
     * @param dval �������ݳ�ʼֵ
     * @param dQ   ϵͳ��������(��Ĭ��ֵ)
     * @param dR   �۲���������(��Ĭ��ֵ)
     */
    void setIniVal(float dval, float dQ = 0.01, float dR = 0.25);

    /**
     * @brief getData ��ȡ�˲�����
     * @param vecReal ��ʵֵ
     * @param vecObserver �۲�ֵ
     * @param vecFilter �˲����ֵ
     */
    void getData(vector<float>& vecReal,
        vector<float>& vecObserver,
        vector<float>& vecFilter);
    // ��ʾ�˲�Ч��
    void displayError();

private:
    // ����-1��1֮��������
    float frand();

private:
    float m_dQ;       // ϵͳ��������
    float m_dR;       // �۲���������
    vector<float> m_vecSysNoise;   // ϵͳ����
    vector<float> m_vecObserNoise; // �۲�����
    vector<float> m_vecReal;   // ��ʵֵ
    vector<float> m_vecObser;  // �۲�ֵ
    vector<float> m_vecKF;     // �˲�ֵ
    vector<float> m_vecCov;    // Э����
};