#include <stdint.h>

#define RCC_BASE    0x40023800
#define GPIOA_BASE  0x40020000
#define USART2_BASE 0x40004400

// RCC
#define RCC_AHB1ENR  (*((volatile uint32_t *)(RCC_BASE + 0x30)))
#define RCC_APB1ENR  (*((volatile uint32_t *)(RCC_BASE + 0x40)))

// GPIOA (TX - PA2)
#define GPIOA_MODER  (*((volatile uint32_t *)(GPIOA_BASE + 0x00)))
#define GPIOA_AFRL   (*((volatile uint32_t *)(GPIOA_BASE + 0x20)))

// USART2
#define USART2_SR    (*((volatile uint32_t *)(USART2_BASE + 0x00)))
#define USART2_DR    (*((volatile uint32_t *)(USART2_BASE + 0x04)))
#define USART2_BRR   (*((volatile uint32_t *)(USART2_BASE + 0x08)))
#define USART2_CR1   (*((volatile uint32_t *)(USART2_BASE + 0x0C)))

void uart_send_char(char c) {
    while (!(USART2_SR & (1 << 7)));  // 전송 준비될 때까지 대기
    USART2_DR = c;
}

void uart_send_string(const char *str) {
    while (*str) {
        uart_send_char(*str++);
    }
}

int main(void) {
    // 클럭 활성화
    RCC_AHB1ENR |= (1 << 0);   // GPIOA 클럭
    RCC_APB1ENR |= (1 << 17);  // USART2 클럭

    // PA2 대체기능 모드 설정 (UART TX)
    GPIOA_MODER &= ~(3 << 4);
    GPIOA_MODER |=  (2 << 4);  // 10: 대체기능 모드

    // PA2 대체기능 AF7 (USART2) 설정
    GPIOA_AFRL &= ~(0xF << 8);
    GPIOA_AFRL |=  (7 << 8);

    // USART2 설정 (9600 baud, 16MHz 기준)
    USART2_BRR = 0x683;   // 9600 baud rate
    USART2_CR1 |= (1 << 3);  // TX 활성화
    USART2_CR1 |= (1 << 13); // USART 활성화

    while(1) {
        uart_send_string("Hello, Embedded!\r\n");
        for (volatile int i = 0; i < 1000000; i++);  // 딜레이
    }
}
