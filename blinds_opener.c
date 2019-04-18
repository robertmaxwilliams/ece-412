

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

void main() 
{
    set_step_size(16);
    while (1) 
    {

        light_sensor_value = analog_read(light_sensor_pin);
        light_sensor_threshold = analog_read(pot_pin);

        target_pos = target_pos(light_sensor_value, light_sensor_threshold);
        target_pos = read_override_state(); //leaves unchanged if not overrode
        move_stepper_to_pos(target_pos);
    }
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
    




        

