#include <stdint.h>

#define RCC_BASE    0x40023800
#define GPIOA_BASE  0x40020000
#define TIM2_BASE   0x40000000

// RCC
#define RCC_AHB1ENR  (*((volatile uint32_t *)(RCC_BASE + 0x30)))
#define RCC_APB1ENR  (*((volatile uint32_t *)(RCC_BASE + 0x40)))

// GPIOA (PA5 - LED)
#define GPIOA_MODER  (*((volatile uint32_t *)(GPIOA_BASE + 0x00)))
#define GPIOA_AFRL   (*((volatile uint32_t *)(GPIOA_BASE + 0x20)))

// TIM2
#define TIM2_CR1     (*((volatile uint32_t *)(TIM2_BASE + 0x00)))
#define TIM2_CCMR1   (*((volatile uint32_t *)(TIM2_BASE + 0x18)))
#define TIM2_CCER    (*((volatile uint32_t *)(TIM2_BASE + 0x20)))
#define TIM2_PSC     (*((volatile uint32_t *)(TIM2_BASE + 0x28)))
#define TIM2_ARR     (*((volatile uint32_t *)(TIM2_BASE + 0x2C)))
#define TIM2_CCR1    (*((volatile uint32_t *)(TIM2_BASE + 0x34)))

int main(void) {
    // 클럭 활성화
    RCC_AHB1ENR |= (1 << 0);   // GPIOA
    RCC_APB1ENR |= (1 << 0);   // TIM2

    // PA5 대체기능 모드 설정
    GPIOA_MODER &= ~(3 << 10);
    GPIOA_MODER |=  (2 << 10); // 대체기능 모드

    // PA5 AF1 (TIM2_CH1) 설정
    GPIOA_AFRL &= ~(0xF << 20);
    GPIOA_AFRL |=  (1 << 20);  // AF1 = TIM2

    // TIM2 설정
    TIM2_PSC  = 16 - 1;        // Prescaler: 16MHz / 16 = 1MHz
    TIM2_ARR  = 1000 - 1;      // Auto Reload: 1MHz / 1000 = 1kHz PWM
    TIM2_CCR1 = 500;           // Duty Cycle 50% (500/1000)

    // PWM 모드 설정
    TIM2_CCMR1 |= (6 << 4);   // PWM 모드 1
    TIM2_CCMR1 |= (1 << 3);   // 프리로드 활성화
    TIM2_CCER  |= (1 << 0);   // CH1 출력 활성화
    TIM2_CR1   |= (1 << 7);   // ARPE 활성화
    TIM2_CR1   |= (1 << 0);   // 타이머 시작

    while(1) {
        // 점점 밝아짐
        for (int i = 0; i <= 1000; i++) {
            TIM2_CCR1 = i;
            for (volatile int j = 0; j < 5000; j++);
        }
        // 점점 어두워짐
        for (int i = 1000; i >= 0; i--) {
            TIM2_CCR1 = i;
            for (volatile int j = 0; j < 5000; j++);
        }
    }
}
