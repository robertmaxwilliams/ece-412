
#include <avr/io.h>
#include <avr/interrupt.h>
 
#define F_CPU 4000000UL
#include <util/delay.h>

int direction_pin = 19;
int step_pin = 18;
int light_sensor_pin = 4;
int pot_pin = 5;

int step_size_pins = {6,7,8};


// set by potentiometer
int light_sensor_threshold = 0.0;

// read in from solar panel
int light_sensor_value = 0.0;

// updated to match motor
int stepper_pos = 0;
int target_pos = 0;
int closed_pos = 0;
int open_pos = 30;

// override switches
int override_open_pin = 13;
int override_closed_pin = 14;


int encoder_a_pin = 0;
int encoder_b_pin = 1;

int encoder_state = 0;
int encoder_pos = 0;



//Interrupt Service Routine for INT0, A pin of encoder
ISR(INT0_vect)
{
    update_encoder();
}



//Interrupt Service Routine for INT1, B pin of encoder
ISR(INT1_vect)
{
    update_encoder();
}

int target_pos(int lsv, int lst)
{
    if (lsv > lst)
    { // if it's bright outside
        return lst - lsv
    }
    return 0
}

int read_override_state()
{
    int override_open = digital_read(override_open_pin);
    int override_closed = digital_read(override_closed_pin);
    if (override_open && override_closed)
        return target_pos + 1// spin mode :D
    else if (override_open)
        return open_pos; //open
    else if (override_closed)
        return closed_pos; //close
    return target_pos; //unchanged
}

void move_stepper_to_pos(int target_pos)
{
    if (target_pos == stepper_pos)
    {
        return
    } 

    if (target_pos > stepper_pos)
    {
        step(0);
    } else if (target_pos < stepper_pos)
    {
        step(1);
    }

    stepper_pos += 1;
    wait_millis(50);
    return
}

void step(int dir)
{
    digital_write(direction_pin, dir);
    digial_write(step_pin, 1);
    delay_ms(1);
    digial_write(step_pin, 0);
}

int read_encoder_state()
{
    int grey_code = 0;
    // read in 2 bit grey code, should be int in 0,1,2,3
    greycode += digital_read(encoder_a_pin);
    greycode += 2 * digital_read(encoder_b_pin)
    int grey_code_to_ordinal[] = {1,2,0,3};
    return grey_code_to_ordinal[greycode]
}

// returns difference between the two, in mod base
// only if they differ by 1. Otherwise returns 0
int mod_diff_by_one(int a, int b, int base)
{
    // modulo is ill-defined in C, and the specifics are confusing,
    // so I'm going to do this in a really dumb way. 
    if ((a+1)%base == b)
        return -1
    if ((b+1)%base == a)
        return 1
    return 0
}

int update_encoder()
{
    int new_encoder_state = read_encoder_state();
    encoder_pos += mod_diff_by_one(new_encoder_state, encoder_state);
    encoder_state = new_encoder_state
}

void main() 
{

    // TODO nintialize pins
    set_step_size(16);
    while (1) 
    {

        light_sensor_value = analog_read(light_sensor_pin);
        light_sensor_threshold = analog_read(pot_pin);

        target_pos = target_pos(light_sensor_value, light_sensor_threshold);
        target_pos = read_override_state(); //leaves unchanged if not overrode
        move_stepper_to_pos(target_pos - encoder_pos);
    }
}

