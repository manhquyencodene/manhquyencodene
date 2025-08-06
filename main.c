void GPIO_Init(void)
{
    // Enable GPIOB clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // Set PB3, PB5, PB7 as output
    GPIOB->MODER &= ~(0b11 << (3 * 2));  // Clear mode PB3
    GPIOB->MODER |=  (0b01 << (3 * 2));  // Set PB3 as output

    GPIOB->MODER &= ~(0b11 << (5 * 2));
    GPIOB->MODER |=  (0b01 << (5 * 2));

    GPIOB->MODER &= ~(0b11 << (7 * 2));
    GPIOB->MODER |=  (0b01 << (7 * 2));
}
void TIM2_Init(void)
{
    // Enable TIM2 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Set Prescaler and ARR
    TIM2->PSC = 8399;         // Prescaler
    TIM2->ARR = 9999;         // Auto-reload

    TIM2->DIER |= TIM_DIER_UIE;  // Enable update interrupt
    TIM2->CR1  |= TIM_CR1_CEN;   // Enable counter

    // Enable TIM2 interrupt in NVIC
    NVIC_EnableIRQ(TIM2_IRQn);
}
volatile uint8_t led_state = 0;

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF) // Check update interrupt flag
    {
        TIM2->SR &= ~TIM_SR_UIF; // Clear flag

        // Tắt hết LED
        GPIOB->ODR &= ~( (1<<3) | (1<<5) | (1<<7) );

        // Bật LED theo trạng thái
        switch (led_state)
        {
            case 0:
                GPIOB->ODR |= (1 << 3); // Red
                break;
            case 1:
                GPIOB->ODR |= (1 << 5); // Green
                break;
            case 2:
                GPIOB->ODR |= (1 << 7); // Blue
                break;
        }

        led_state = (led_state + 1) % 3;
    }
}
int main(void)
{
    GPIO_Init();   // Bật GPIOB, config output
    TIM2_Init();   // Cấu hình timer

    while(1)
    {
        // Không làm gì cả, chờ ngắt
    }
}

