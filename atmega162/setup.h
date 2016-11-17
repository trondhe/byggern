#pragma once

//Added from the project description to help setting bits in registers
#define s_bit(reg,bit) (reg |= (1 << bit))
#define c_bit(reg,bit) (reg &= ~(1 << bit))
#define t_bit_h( reg, bit ) (reg & (1 << bit))
#define t_bit_l( reg, bit ) (!(reg & (1 << bit)))