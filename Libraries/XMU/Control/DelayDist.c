#include "headfile.h"

//================================================================//
//  @brief  :		延距判断环岛
//  @param  :		void
//  @return :		void
//  @note   :		放在中断
//================================================================//
void DistCI(void)
{
  static int CI_sum_dist = 7000;
  static unsigned char CI_enable = 0;     //判断使能
  static int CI_acc_dist = 0;
  if (1 == CircleState)
    CI_enable = 1;
  if (CI_enable)                
  {
    CI_acc_dist += curSpeed;
    if (CI_acc_dist > CI_sum_dist)
    {
      CircleFlag = 0;
      CircleState = 0;
      CI_enable = 0;
      CI_acc_dist = 0;
    }
    else if (left_norm > 1 || right_norm > 1)
    {
      CI_enable = 0;
      CI_acc_dist = 0;
    }
    else;
  }
}

//================================================================//
//  @brief  :		电磁判断入环岛
//  @param  :		void
//  @return :		void
//  @note   :		放在中断
//================================================================//
void IndCI(void)
{
  if (right_norm > 1 && !CircleFlag)
  {
    CircleFlag = 2;
    CircleState = 3;
//    gpio_init(D0,GPO,0);
  }
  if (left_norm > 1 && !CircleFlag)
  {
    CircleFlag = 1;
    CircleState = 3;
//    gpio_init(D0,GPO,0);
  }
}
      
//================================================================//
//  @brief  :		延距判断断路
//  @param  :		void
//  @return :		void
//  @note   :		放在中断
//================================================================//
void DistBroken(void)
{
  static int BR_sum_dist = 5000;
  static int BR_acc_dist = 0;
  static unsigned char BR_enable = 0;
  if (BrokenFlag==1)
    BR_enable = 1;
  if (BR_enable)
  {
    BR_acc_dist += curSpeed;
    if (BrokenFlag==2)
    {
      BR_enable = 0;
      BR_acc_dist = 0;
    }
    else if (BR_acc_dist > BR_sum_dist)
    {
      BrokenFlag = 0;
      BR_enable = 0;
      BR_acc_dist = 0;
    }
  }
}