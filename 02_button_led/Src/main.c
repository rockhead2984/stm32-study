#include <stdint.h>

#define RCC_BASE    0x40023800
#define GPIOA_BASE  0x40020000
#define GPIOC_BASE  0x40020800

// 클럭
#define RCC_AHB1ENR  (*((volatile uint32_t *)(RCC_BASE + 0x30)))

// GPIOA (LED - PA5)
#define GPIOA_MODER  (*((volatile uint32_t *)(GPIOA_BASE + 0x00)))
#define GPIOA_ODR    (*((volatile uint32_t *)(GPIOA_BASE + 0x14)))

// GPIOC (버튼 - PC13)
#define GPIOC_MODER  (*((volatile uint32_t *)(GPIOC_BASE + 0x00)))
#define GPIOC_IDR    (*((volatile uint32_t *)(GPIOC_BASE + 0x10)))

int main(void)
{
    // GPIOA, GPIOC 클럭 활성화
    RCC_AHB1ENR |= (1 << 0);  // GPIOA
    RCC_AHB1ENR |= (1 << 2);  // GPIOC

    // PA5 출력 모드 설정 (LED)
    GPIOA_MODER &= ~(3 << 10);
    GPIOA_MODER |=  (1 << 10);

    // PC13 입력 모드 설정 (버튼) - 기본값이 입력이라 안 써도 됨
    GPIOC_MODER &= ~(3 << 26);

    while(1) {
        // 버튼 눌리면 PC13이 0이 됨 (Active Low)
        if (!(GPIOC_IDR & (1 << 13))) {
            GPIOA_ODR |=  (1 << 5);  // LED ON
        } else {
            GPIOA_ODR &= ~(1 << 5);  // LED OFF
        }
    }
}
