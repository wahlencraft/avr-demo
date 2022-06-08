/* This is a header declaring the functions defined by assembly rutines in
 * time.S. */

#ifndef TIME_H_
#define TIME_H_

extern uint16_t read_timer0();
extern uint16_t read_timer1();

/* (Re)start counter 0/1 with a prescaler 1024 */
extern void start_counter0();
extern void start_counter1();

/* Use counter 0 timer A to perform a busy wait
 *
 * Counter 0 must be started first (with prescaler 1024)
 * The counter is not reset due to this call. So it can also be used for other
 * things.
 *
 * Note that the rage of allowed t depends on the system clock frequency
 * 1 < F_CPU*t/1,000,000 < 257
 * Also note that the system clock must be a multiple of 1,000,000
 */
extern void busy_wait_ms0(uint8_t t);

/* Use counter 1 timer A to perform a busy wait
 *
 * Counter 1 must be started first (with prescaler 1024)
 * The counter is not reset due to this call. So it can also be used for other
 * things.
 *
 * Note that the rage of allowed t depends on the system clock frequency
 * 1 < F_CPU*t/1,000,000 < 21,845
 * Also note that the system clock must be a multiple of 1,000,000
 */
extern void busy_wait_ms1(uint16_t t);

#endif // TIME_H_

