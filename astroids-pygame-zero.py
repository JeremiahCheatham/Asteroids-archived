#!/usr/bin/env python

import pgzrun
import math
from pgzero.builtins import Actor, keyboard, keys, sounds, clock, music
from random import randint

WIDTH = 800
HEIGHT = 600
CENTERX = WIDTH / 2
CENTERY = HEIGHT / 2
TITLE = "Astroids - Pygame Zero"
ICON = "images/icon.png"
MUSIC = "falling"

game_mode = "intro"
level = 1
STARTING_LIVES = 1
lives = STARTING_LIVES
score = 0

THRUST = 0.1
SHIP_DIA = 25
engines = False
immunity = False
immunity_counter = 0

shotcount = 100
SHOTLIFE = 40
SHOT_DIA = 5
shots = []

ASTROID_L = 58
ASTROID_M = 26
ASTROID_S = 12
astroids = []


class Ship(Actor):
    def __init__(self):
        super().__init__("ship")
        self.angle = 0
        self.pos = CENTERX, CENTERY
        self.xvel = 0
        self.yvel = 0
        self.diameter = SHIP_DIA

    def update_vector(self):
        radian = math.radians(self.angle)
        self.xvel += math.sin(radian) * THRUST
        self.yvel += math.cos(radian) * THRUST

    def reset(self):
        self.angle = 0
        self.pos = CENTERX, CENTERY
        self.xvel = 0
        self.yvel = 0


class Shot(Actor):
    def __init__(self, angle, pos, count):
        super().__init__("shot")
        self.lifetime = 0
        self.count = count
        self.angle = angle
        self.pos = pos
        self.diameter = SHOT_DIA
        radian = math.radians(angle)
        self.xvel = math.sin(radian) * 10
        self.yvel = math.cos(radian) * 10
        self.x -= self.xvel * 3
        self.y -= self.yvel * 3


class Astroid(Actor):
    def __init__(self, astroid_size, angle, speed, spin):
        super().__init__("astroid" + astroid_size)
        self.angle = angle
        self.spin = spin
        self.astroid_size = astroid_size
        radian = math.radians(angle)
        self.xvel = math.sin(radian) * speed
        self.yvel = math.cos(radian) * speed
        if astroid_size == "large":
            self.diameter = ASTROID_L
        elif astroid_size == "medium":
            self.diameter = ASTROID_M
        else:
            self.diameter = ASTROID_S


def toggle_immunity():
    global immunity, immunity_counter
    if immunity:
        immunity = False
    else:
        immunity = True
        immunity_counter = 0
        clock.schedule(toggle_immunity, 3)


def create_astroids(num):
    global astroids
    astroids = []
    for astroid in range(0, num):
        angle = randint(-180, 180)
        spin = randint(-100, 100) / 100
        speed = randint(1, 10) / 10
        astroid = Astroid("large", angle, speed, spin)
        dist = 0
        while dist < 200:
            astroid.pos = randint(0, WIDTH), randint(0, HEIGHT)
            dist = math.hypot(astroid.x - CENTERX, astroid.y - CENTERY)
        astroids.append(astroid)


def create_level():
    global shots
    shots = []
    ship.reset()
    create_astroids(level)
    if not immunity:
        toggle_immunity()


def mode_select():
    global game_mode, level, lives, score, shotcount, messege
    if game_mode == "intro":
        messege = "ASTROIDS!"
        level = 1
        lives = STARTING_LIVES
        score = 0
        game_mode = "level"
    elif game_mode == "level":
        messege = "LEVEL: " + str(level)
        shotcount = 100
        game_mode = "play"
    elif game_mode == "play":
        create_level()
        game_mode = "playing"
        return
    elif game_mode == "over":
        messege = "GAME OVER!"
        game_mode = "score"
    elif game_mode == "score":
        messege = "SCORE: " + str(score)
        game_mode = "intro"
    clock.schedule(mode_select, 5)


def shoot():
    global shots, shotcount
    if len(shots) < 5:
        shot = Shot(ship.angle, ship.pos, shotcount)
        if shotcount:
            shotcount -= 1
        shots.append(shot)
        sounds.laser3.play()


def on_key_down(key):
    if key == keys.SPACE:
        shoot()
    if key == keys.M:
        if music.is_playing(MUSIC):
            music.pause()
        else:
            music.unpause()


def check_off_screen(actor):
    if actor.x + actor.diameter < 0:
        actor.x = WIDTH + actor.diameter
        actor.y = HEIGHT - actor.y
    elif actor.x - actor.diameter > WIDTH:
        actor.x = 0 - actor.diameter
        actor.y = HEIGHT - actor.y
    if actor.y + actor.diameter < 0:
        actor.y = HEIGHT + actor.diameter
        actor.x = WIDTH - actor.x
    elif actor.y - actor.diameter > HEIGHT:
        actor.y = 0 - actor.diameter
        actor.x = WIDTH - actor.x


def update_ship():
    ship.x -= ship.xvel
    ship.y -= ship.yvel
    check_off_screen(ship)


def update_shots():
    if shots:
        for shot in shots:
            shot.lifetime += 1
            if shot.lifetime > SHOTLIFE:
                shots.remove(shot)
            else:
                shot.x -= shot.xvel
                shot.y -= shot.yvel
                check_off_screen(shot)


def update_astroids():
    if astroids:
        for astroid in astroids:
            astroid.angle += astroid.spin
            astroid.x -= astroid.xvel
            astroid.y -= astroid.yvel
            check_off_screen(astroid)


def split_astroid(angle, pos, size):
    angle += 90
    for i in range(2):
        spin = randint(-100, 100) / 100
        speed = randint(3, 12) / 10
        astroid = Astroid(size, angle, speed, spin)
        astroid.pos = pos
        astroids.append(astroid)
        angle += 180


def check_shot_collision():
    global level, score, game_mode, lives
    for shot in shots:
        for astroid in astroids:
            dist = math.hypot(astroid.x - shot.x, astroid.y - shot.y)
            if dist < astroid.diameter:
                score += shot.count * level
                if astroid.astroid_size == "large":
                    split_astroid(shot.angle, astroid.pos, "medium")
                elif astroid.astroid_size == "medium":
                    split_astroid(shot.angle, astroid.pos, "small")
                shots.remove(shot)
                astroids.remove(astroid)
                sounds.collide.play()
                if not astroids:
                    sounds.thrusters.stop()
                    level += 1
                    lives += 1
                    game_mode = "level"
                    mode_select()
                return


def check_ship_collision():
    global game_mode, lives
    for astroid in astroids:
        dist = math.hypot(astroid.x - ship.x, astroid.y - ship.y)
        if dist < astroid.diameter + ship.diameter:
            sounds.boom.play()
            toggle_immunity()
            if not lives:
                sounds.thrusters.stop()
                game_mode = "over"
                mode_select()
            else:
                lives -= 1


def update():
    global engines, immunity_counter
    if game_mode == "playing":
        if keyboard.right or keyboard.d:
            ship.angle -= 5
        if keyboard.left or keyboard.a:
            ship.angle += 5
        if keyboard.up or keyboard.w:
            ship.update_vector()
            if not engines:
                engines = True
                ship.image = "ship2"
                ship.angle = ship.angle
                sounds.thrusters.play(-1)
        else:
            if engines:
                engines = False
                ship.image = "ship"
                ship.angle = ship.angle
                sounds.thrusters.stop()
        update_ship()
        update_shots()
        update_astroids()
        check_shot_collision()
        if immunity:
            if immunity_counter > 9:
                immunity_counter = 0
            else:
                immunity_counter += 1
        else:
            check_ship_collision()


def draw():
    screen.blit("backdrop", (0, 0))
    if game_mode == "playing":
        if not (immunity and immunity_counter < 5):
            ship.draw()
        for shot in shots:
            shot.draw()
        for astroid in astroids:
            astroid.draw()
        for i in range(1, lives + 1):
            screen.blit("icon", (30 * i, 10))
        screen.draw.text("SCORE: " + str(score), topright=(WIDTH - 34, 10), color="white", fontsize=30)
    else:
        screen.draw.text(messege, center=(CENTERX, CENTERY), color="white", fontsize=120)


ship = Ship()
create_level()
mode_select()
music.play(MUSIC)

pgzrun.go()
