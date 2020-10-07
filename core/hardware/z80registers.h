#pragma once

#include <core/hardware/cpu.h>

#include <QTableView>
#include <QCheckBox>

namespace hw
{
using reg8 = uint8_t;
using byte = Memory::byte_t;

struct reg16u
{
    reg16u();

    uint16_t val;
    uint8_t& hi() { return *(reinterpret_cast<uint8_t*>(&val)+1); }
    uint8_t& lo() { return *(reinterpret_cast<uint8_t*>(&val)); }

};

union flags
{
    uint8_t f;
    struct
    {
        unsigned c: 1, n: 1, pv: 1, u3: 1, h: 1, u5: 1, z: 1, s: 1;
    };
};

struct regaf
{
    uint16_t val;
    uint8_t& a() { return *((uint8_t*)(&val)+1); }
    flags& f() { return *((flags*)(&val)); }
};

class FlagCheckBox : public QCheckBox
{
public:
    FlagCheckBox(reg8& flags, uint8_t mask, QWidget* label);
    virtual ~FlagCheckBox()=default;

    void update()
    {
        QPalette pal = label->palette();

        bool old = isChecked();
        if (old != (flags & mask))
            pal.setColor(QPalette::WindowText, Qt::red);
        else
            pal.setColor(QPalette::WindowText, Qt::black);
        label->setPalette(pal);
        setChecked(flags & mask);
    }

private:
    QWidget* label;
    reg8& flags;
    uint8_t mask;
};


struct Z80Registers : public Cpu::Registers
{
    Z80Registers() :
          a(af.a()),
          b(bc.hi()),
          c(bc.lo()),
          d(de.hi()),
          e(de.lo()),
          f(af.f()),
          h(hl.hi()),
          l(hl.lo()) {}

    virtual ~Z80Registers() override = default;

    uint16_t get(const std::string reg) override;

    reg16   pc;
    reg16   sp;
    reg16   ix;
    reg16   iy;
    regaf   af;
    reg16u  bc;
    reg16u  de;
    reg16u  hl;
    regaf   af2;
    reg16u  bc2;
    reg16u  de2;
    reg16u  hl2;
    reg8    i;
    reg8    r;

    reg8&	a;
    reg8&	b;
    reg8&	c;
    reg8&	d;
    reg8&	e;
    flags&   f;
    reg8&	h;
    reg8&	l;

    bool set(std::string reg, int32_t value) override;

    std::string serialize() const override;

    // TODO Design : should really not merge registers with view of registers
    void update() override;
    QWidget* createViewForm(QWidget* parent) override;
    QTableView* table = nullptr;
    FlagCheckBox* flag_s;
    FlagCheckBox* flag_z;
    FlagCheckBox* flag_5;
    FlagCheckBox* flag_h;
    FlagCheckBox* flag_3;
    FlagCheckBox* flag_pv;
    FlagCheckBox* flag_n;
    FlagCheckBox* flag_c;
};

} // ns hw

