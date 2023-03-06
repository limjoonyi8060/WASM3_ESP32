(module
  (type $t0 (func (param i32 i32)))
  (type $t1 (func (result i32)))
  (type $t2 (func (param i32)))
  (type $t3 (func))
  (import "arduino" "print" (func $arduino.print (type $t0)))
  (import "arduino" "getPinLED" (func $arduino.getPinLED (type $t1)))
  (import "arduino" "pinMode" (func $arduino.pinMode (type $t0)))
  (import "arduino" "getGreeting" (func $arduino.getGreeting (type $t0)))
  (import "arduino" "printn" (func $arduino.printn (type $t2)))
  (func $_start (type $t3)
    (local $l0 i32) (local $l1 i32)
    global.get $g0
    i32.const -64
    i32.add
    local.tee $l0
    global.set $g0
    i32.const 1087
    i32.const 1
    call $arduino.print
    i32.const 1028
    i32.const 16
    call $arduino.print
    i32.const 1024
    call $arduino.getPinLED
    local.tee $l1
    i32.store
    local.get $l1
    i32.const 1
    call $arduino.pinMode
    i32.const 1087
    i32.const 1
    call $arduino.print
    i32.const 1045
    i32.const 10
    call $arduino.print
    local.get $l0
    i32.const 64
    call $arduino.getGreeting
    i32.const 1087
    i32.const 1
    call $arduino.print
    local.get $l0
    i32.const 1
    i32.sub
    local.set $l1
    loop $L0
      local.get $l1
      i32.const 1
      i32.add
      local.tee $l1
      i32.load8_u
      br_if $L0
    end
    local.get $l0
    local.get $l1
    local.get $l0
    i32.sub
    call $arduino.print
    local.get $l0
    i32.const -64
    i32.sub
    global.set $g0
    i32.const 27215
    local.set $l0
    loop $L1
      i32.const 1087
      i32.const 1
      call $arduino.print
      i32.const 1056
      i32.const 11
      call $arduino.print
      local.get $l0
      i32.const 27315
      i32.sub
      call $arduino.printn
      i32.const 1068
      i32.const 8
      call $arduino.print
      local.get $l0
      call $arduino.printn
      i32.const 1087
      i32.const 1
      call $arduino.print
      i32.const 1077
      i32.const 9
      call $arduino.print
      local.get $l0
      i32.const 100
      i32.add
      local.tee $l0
      i32.const 29415
      i32.ne
      br_if $L1
    end)
  (memory $memory 1)
  (global $g0 (mut i32) (i32.const 5200))
  (export "memory" (memory $memory))
  (export "_start" (func $_start))
  (data $d0 (i32.const 1028) "I am Daniel \f0\9f\98\8e\00Greeting: \00 Celsius = \00 Kelvin=\00---------\00\0a"))
