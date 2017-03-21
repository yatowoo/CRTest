/*
*   File : CRTest/inc/RNGWrapper.hh
*
*   Brief: Util Random Function Wrapper  for CRY@LLNL package
*
*   Reference : $CRYHOME/geant4/include/RNGWrapper.hh
*/

#ifndef CRTest_RNGWrapper_h
#define CRTest_RNGWrapper_h

template <class T>
class RNGWrapper
{
  public:
    static void set(T *object, double (T::*func)(void));
    static double rng(void);

  private:
    static T *m_obj;
    static double (T::*m_func)(void);
};

template <class T>
T *RNGWrapper<T>::m_obj;

template <class T>
double (T::*RNGWrapper<T>::m_func)(void);

template <class T>
void RNGWrapper<T>::set(T *object, double (T::*func)(void))
{
    m_obj = object;
    m_func = func;
}

template <class T>
double RNGWrapper<T>::rng(void)
{
    return (m_obj->*m_func)();
}

#endif // CRTest_RNGWrapper_h