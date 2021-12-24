eps = 0.005; // Allow for overlaps/gaps

// Box dimensions
box_inner_x = 150;
box_inner_y = 110;
box_inner_z = 45;
box_outer_x = 175;
box_outer_y = 129;
box_outer_z = 50;

// Lid dimensions
lid_inner_x = 175;
lid_inner_y = 131;
lid_inner_z = 10;
lid_outer_x = 182;
lid_outer_y = 138;
lid_outer_z = 16;

// Lid hole positions
hole_diameter = 12;
hole_gap = 57;
hole_offset = -3;

// Servo positions
servo_offset = 30;
servo_height = 0;
servo_1_angle = 90;
servo_2_angle = 0;

// Box centered on the origin, with z0 at the bottom of the interior
box(
    inner_x = box_inner_x, inner_y = box_inner_y, inner_z = box_inner_z,
    outer_x = box_outer_x, outer_y = box_outer_y, outer_z = box_outer_z
);
// lid(
//     box_height = box_inner_z,
//     inner_x = lid_inner_x, inner_y = lid_inner_y, inner_z = lid_inner_z,
//     outer_x = lid_outer_x, outer_y = lid_outer_y, outer_z = lid_outer_z
// );
lid_with_holes(
    box_height = box_inner_z,
    inner_x = lid_inner_x, inner_y = lid_inner_y, inner_z = lid_inner_z,
    outer_x = lid_outer_x, outer_y = lid_outer_y, outer_z = lid_outer_z,
    hole_diameter = hole_diameter, hole_gap = hole_gap, hole_offset = hole_offset
);

// Arduino positioned center end
// 80 = Arduino frame x
translate([-(box_inner_x - 80) / 2, 0, 0])
arduino();

// Two switches
translate([hole_offset, hole_gap / 2, box_inner_z])
switch(
    color = "red",
    state = true
);
translate([hole_offset, -hole_gap / 2, box_inner_z])
switch(
    color = "green"
);

// Servos
// 5.5 = servo width
translate([servo_offset, box_inner_y / 2, 5.5 + servo_height])
rotate([90, 0, 0])
union() {
    servo(
        angle = servo_1_angle
    );
    translate([0, 0, 31])
    servo_arm(
        angle = servo_1_angle
    );
}

translate([servo_offset, -box_inner_y / 2, 5.5 + servo_height])
rotate([-90, 0, 0])
union() {
    servo(
        angle = servo_2_angle
    );
    translate([0, 0, 31])
    servo_arm(
        angle = servo_2_angle,
        mirror = true
    );
}

module box(
    inner_x, inner_y, inner_z,
    outer_x, outer_y, outer_z,
) {
    %color("blue", 0.45)
    difference() {
        // Outer
        translate([-outer_x / 2, -outer_y / 2, (inner_z - outer_z) - eps])
        cube([outer_x, outer_y, outer_z]);
        // Inner
        translate([-inner_x / 2, -inner_y / 2, 0])
        cube([inner_x, inner_y, inner_z]);
    }
}

module lid(
    box_height,
    inner_x, inner_y, inner_z,
    outer_x, outer_y, outer_z,
) {
    color("white", 0.45)
    difference() {
        // Outer
        translate([-outer_x / 2, -outer_y / 2, (box_height - inner_z) + eps + eps])
        cube([outer_x, outer_y, outer_z]);
        // Inner
        translate([-inner_x / 2, -inner_y / 2, (box_height - inner_z) + eps])
        cube([inner_x, inner_y, inner_z]);
    }
}

module lid_with_holes(
    box_height,
    inner_x, inner_y, inner_z,
    outer_x, outer_y, outer_z,
    hole_diameter, hole_gap, hole_offset,
) {

    %color("white", 0.55)
    difference() {
        lid(
            box_height = box_height,

            inner_x = inner_x,
            inner_y = inner_y,
            inner_z = inner_z,

            outer_x = outer_x,
            outer_y = outer_y,
            outer_z = outer_z
        );

        // Left hole
        translate([hole_offset, hole_gap / 2, box_height - eps])
        cylinder(h=10, r=hole_diameter / 2);

        // Right hole
        translate([hole_offset, -hole_gap / 2, box_height - eps])
        cylinder(h=10, r=hole_diameter / 2);
    }
}

module arduino() {
    tray_x = 80;
    tray_y = 55;
    tray_z = 5;

    arduino_x = 68.6;
    arduino_y = 53.4;
    arduino_z = 20;

    union() {
        // Transparent base tray
        color("white", 0.55)
        translate([-tray_x / 2, -tray_y / 2, 0])
        cube([tray_x, tray_y, tray_z]);

        // Arduino board itself
        color("green")
        translate([-38, -arduino_y / 2, tray_z / 2])
        cube([arduino_x, arduino_y, arduino_z]);
    }
}

module switch(
    color = "yellow",
    state = false,
) {
    union() {
        // Base
        color(color, 0.45)
        translate([-13, -6, -14])
        cube([26, 12, 14]);

        // Neck
        color("black")
        cylinder(h = 7, r = 6);

        // Toggle
        color(color, 0.45)
        translate([0, 0, 5.5])
        rotate([0, 15 * (state ? 1 : -1), 0])
        translate([0, 0, -5.5])
        translate([-2, -5.5, 6])
        cube([4, 11, 28]);

        // Connectors
        color("blue")
        translate([-20, -4, -38])
        cube([30, 8, 24]);
    }
}

module servo(
    angle = 0
) {
    union() {
        // Body
        color("blue")
        translate([-5, -5.5, 0])
        cube([22, 11, 22]);

        color("blue")
        translate([0, 0, 22])
        cylinder(r = 5.5, h = 4);

        // Arm
        color("white")
        rotate([0, 0, angle])
        translate([-3.5, -3.5, 26])
        cube([20, 7, 5]);
    }
}

module servo_arm(
    angle = 0,
    mirror = false,
) {
    rotate([0, 0, angle])
    mirror([0, (mirror ? 1 : 0), 0])
    union(){
        color("yellow")
        translate([0, -5, 0])
        cube([40, 10, 10]);

        color("yellow")
        translate([35, -5, -5])
        rotate([0, 0, 45])
        cube([40, 10, 10]);
    }
}
