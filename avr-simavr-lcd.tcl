# Utility: count bits set in an integer
proc bitcount {n} {
    set count 0
    while {$n > 0} {
        incr count [expr {$n & 1}]
        set n [expr {$n >> 1}]
    }
    return $count
}

# Parse avr_vcd_add_signal call text
proc parse_add_signal {call} {

    set result {}

    if {[regexp {,\s*([0-9]+)\s*(?:/\*.*?\*/)?\s*,\s*"([^"]+)"} \
            $call -> bits name]} {
        set result $name\[$bits\]
    }

    return $result
}

# Parse symbols from file
# Returns list of {name width}
proc symbolsListFromFile {filename} {

    set fh [open $filename r]

    set symbols {}
    set collecting 0
    set buffer ""

    set pending_symbol ""

    while {[gets $fh line] >= 0} {

        # avr_vcd_add_signal
        if {!$collecting && [string match *avr_vcd_add_signal* $line]} {
            set collecting 1
            set buffer $line

            if {[string match "*);*" $line]} {
                set collecting 0
                set r [parse_add_signal $buffer]
                if {$r ne ""} { lappend symbols $r }
            }
            continue
        }

        if {$collecting} {
            append buffer " " $line
            if {[string match "*);*" $line]} {
                set collecting 0
                set r [parse_add_signal $buffer]
                if {$r ne ""} { lappend symbols $r }
            }
            continue
        }

        # AVR_MCU_VCD_SYMBOL
        if {[regexp {AVR_MCU_VCD_SYMBOL\s*\(\s*"([^"]+)"} $line -> name]} {
            set pending_symbol $name
            continue
        }

        # mask parsing
        if {$pending_symbol ne ""} {

            if {[regexp {\.mask\s*=\s*0x([0-9A-Fa-f]+)} $line -> hex]} {
                set width [bitcount [expr 0x$hex]]
                lappend symbols $pending_symbol\[$width\]
                set pending_symbol ""
                continue
            }

            if {[regexp {\.mask\s*=\s*\(\s*1\s*<<\s*[0-9]+\s*\)} $line]} {
                lappend symbols $pending_symbol\[1\]
                set pending_symbol ""
                continue
            }
        }
    }

    close $fh
    return $symbols
}

# Get filename from environment variable
if {![info exists ::env(SIMAVR_VCD_SOURCE_FILE)]} {
    puts stderr "Error: environment variable SIMAVR_VCD_SOURCE_FILE not set"
    exit 1
}
set filename $::env(SIMAVR_VCD_SOURCE_FILE)

set signal_list [symbolsListFromFile $filename]
foreach {signal} $signal_list {
    puts "Found signal - $signal"
}
gtkwave::addSignalsFromList $signal_list
gtkwave::setZoomFactor 0
