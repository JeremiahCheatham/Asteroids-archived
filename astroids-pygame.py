#!/usr/bin/env python

import pygame
import pygame.freetype
import math
from random import randint
from pygame.locals import (K_LEFT, K_RIGHT, K_UP)

pygame.init()

WIDTH = 800
HEIGHT = 600
CENTERX = WIDTH / 2
CENTERY = HEIGHT / 2
TITLE = "Astroids - Pygame"
ICON = "images/icon.png"
FPS = 60

screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption(TITLE)
pygame.display.set_icon(pygame.image.load(ICON).convert_alpha())

background = pygame.image.load("images/backdrop.jpg").convert()
life = pygame.image.load("images/icon.png").convert_alpha()
thrusters = pygame.mixer.Sound("sounds/thrusters.ogg")
laser = pygame.mixer.Sound("sounds/laser3.ogg")
boom = pygame.mixer.Sound("sounds/boom.ogg")
collide = pygame.mixer.Sound("sounds/collide.ogg")
music = pygame.mixer.music.load("music/falling.ogg")

game_mode = "intro"
mode_select_timer = pygame.USEREVENT+2
level = 1
STARTING_LIVES = 1
lives = STARTING_LIVES
score = 0
running = True

THRUST = 0.1
SHIP_DIA = 25
engines = False
immunity = False
immunity_counter = 0
immunity_timer = pygame.USEREVENT+1

shotcount = 100
SHOTLIFE = 40
SHOT_DIA = 5
shots = []

ASTROID_L = 58
ASTROID_M = 26
ASTROID_S = 12
astroids = []


class Ship(pygame.sprite.Sprite):
    def __init__(self):
        super().__init__()
        self.ship_surf = pygame.image.load("images/ship.png").convert_alpha()
        self.engine_surf = pygame.image.load("images/ship2.png").convert_alpha()
        self.orig_surf = self.ship_surf
        self.surf = self.ship_surf
        self.rect = self.surf.get_rect(center=(CENTERX, CENTERY))
        self.x, self.y = self.rect.center
        self._angle = 0
        self.xvel = 0
        self.yvel = 0
        self.diameter = SHIP_DIA

    @property
    def angle(self):
        return self._angle

    @angle.setter
    def angle(self, angle):
        self._angle = angle % 360
        self.surf = pygame.transform.rotate(self.orig_surf, self._angle)
        self.rect = self.surf.get_rect(center=(self.x, self.y))

    def engine(self, show):
        if show:
            self.orig_surf = self.engine_surf
        else:
            self.orig_surf = self.ship_surf
        self.surf = pygame.transform.rotate(self.orig_surf, self._angle)
        self.rect = self.surf.get_rect(center=(self.x, self.y))

    def update_vector(self):
        radian = math.radians(self.angle)
        self.xvel += math.sin(radian) * THRUST
        self.yvel += math.cos(radian) * THRUST

    def reset(self):
        self.angle = 0
        self.rect.center = CENTERX, CENTERY
        self.x, self.y = self.rect.center
        self.xvel = 0
        self.yvel = 0


class Shot(pygame.sprite.Sprite):
    def __init__(self, angle, pos, count):
        super().__init__()
        radian = math.radians(angle)
        self.xvel = math.sin(radian) * 10
        self.yvel = math.cos(radian) * 10
        self.x, self.y = pos
        self.x -= self.xvel * 3
        self.y -= self.yvel * 3
        self.surf = pygame.image.load("images/shot.png").convert_alpha()
        self.surf = pygame.transform.rotate(self.surf, angle)
        self.rect = self.surf.get_rect(center=(self.x, self.y))
        self.x, self.y = self.rect.center
        self.lifetime = 0
        self.count = count
        self.angle = angle
        self.diameter = SHOT_DIA


class Astroid(pygame.sprite.Sprite):
    def __init__(self, astroid_size, angle, speed, spin):
        super().__init__()
        self.og_surf = pygame.image.load("images/astroid" + astroid_size + ".png").convert_alpha()
        self.surf = self.og_surf
        self.rect = self.surf.get_rect()
        self.x, self.y = self.rect.center
        self._angle = angle
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

    @property
    def angle(self):
        return self._angle

    @angle.setter
    def angle(self, angle):
        self._angle = angle % 360
        self.surf = pygame.transform.rotate(self.og_surf, self._angle)
        self.rect = self.surf.get_rect(center=(self.x, self.y))


class Text():
    def __init__(self, text, size, anchor, pos):
        self.font = pygame.freetype.Font(None, size)
        self.surf, self.rect = self.font.render(text, (255, 255, 255))
        self.anchor = anchor
        if self.anchor == "center":
            self.rect.center = pos
        elif self.anchor == "topright":
            self.rect.topright = pos

    def text(self, text):
        old_rect = self.rect
        self.surf, self.rect = self.font.render(text, (255, 255, 255))
        if self.anchor == "center":
            self.rect.center = old_rect.center
        elif self.anchor == "topright":
            self.rect.topright = old_rect.topright


def toggle_immunity():
    global immunity, immunity_counter
    if immunity:
        immunity = False
    else:
        immunity = True
        immunity_counter = 0
        pygame.time.set_timer(immunity_timer, 3000, True)


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
            astroid.x, astroid.y = randint(0, WIDTH), randint(0, HEIGHT)
            dist = math.hypot(astroid.x - CENTERX, astroid.y - CENTERY)
        astroid.rect.center = astroid.x, astroid.y
        astroids.append(astroid)


def create_level():
    global shots
    shots = []
    ship.reset()
    create_astroids(level)
    if not immunity:
        toggle_immunity()


def mode_select():
    global game_mode, level, lives, score, shotcount
    if game_mode == "intro":
        level = 1
        lives = STARTING_LIVES
        score = 0
        messege_text.text("ASTROIDS!")
        game_mode = "level"
    elif game_mode == "level":
        shotcount = 100
        messege_text.text("LEVEL: " + str(level))
        game_mode = "play"
    elif game_mode == "play":
        create_level()
        game_mode = "playing"
        return
    elif game_mode == "over":
        messege_text.text("GAME OVER!")
        game_mode = "score"
    elif game_mode == "score":
        messege_text.text("SCORE: " + str(score))
        game_mode = "intro"
    pygame.time.set_timer(mode_select_timer, 5000, True)


def shoot():
    global shots, shotcount
    if game_mode == "playing":
        if len(shots) < 5:
            shot = Shot(ship.angle, ship.rect.center, shotcount)
            if shotcount:
                shotcount -= 1
            shots.append(shot)
            pygame.mixer.Sound.play(laser)


def update_input():
    global running, engines
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False
            elif event.key == pygame.K_SPACE:
                shoot()
            elif event.key == pygame.K_m:
                if pygame.mixer.music.get_busy():
                    pygame.mixer.music.pause()
                else:
                    pygame.mixer.music.unpause()
        elif event.type == mode_select_timer:
            mode_select()
        elif event.type == immunity_timer:
            toggle_immunity()

    if game_mode == "playing":
        keys = pygame.key.get_pressed()
        if keys:
            if keys[K_LEFT] and not keys[K_RIGHT]:
                ship.angle += 5
            if keys[K_RIGHT] and not keys[K_LEFT]:
                ship.angle -= 5
            if keys[K_UP]:
                ship.update_vector()
                if not engines:
                    engines = True
                    ship.engine(True)
                    pygame.mixer.Sound.play(thrusters, loops=-1)
            else:
                if engines:
                    engines = False
                    ship.engine(False)
                    pygame.mixer.Sound.stop(thrusters)


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
    ship.rect.center = ship.x, ship.y


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
                shot.rect.center = shot.x, shot.y


def update_astroids():
    if astroids:
        for astroid in astroids:
            astroid.angle += astroid.spin
            astroid.x -= astroid.xvel
            astroid.y -= astroid.yvel
            check_off_screen(astroid)
            astroid.rect.center = astroid.x, astroid.y


def split_astroid(angle, pos, size):
    angle += 90
    for i in range(2):
        spin = randint(-100, 100) / 100
        speed = randint(3, 12) / 10
        astroid = Astroid(size, angle, speed, spin)
        astroid.rect.center = pos
        astroid.x, astroid.y = pos
        astroids.append(astroid)
        angle += 180


def check_shot_collision():
    global level, score, game_mode, lives
    for shot in shots:
        for astroid in astroids:
            dist = math.hypot(astroid.x - shot.x, astroid.y - shot.y)
            if dist < astroid.diameter:
                score += shot.count * level
                score_text.text("SCORE: " + str(score))
                if astroid.astroid_size == "large":
                    split_astroid(shot.angle, astroid.rect.center, "medium")
                elif astroid.astroid_size == "medium":
                    split_astroid(shot.angle, astroid.rect.center, "small")
                shots.remove(shot)
                astroids.remove(astroid)
                pygame.mixer.Sound.play(collide)
                if not astroids:
                    pygame.mixer.Sound.stop(thrusters)
                    level += 1
                    lives += 1
                    game_mode = "level"
                    mode_select()
                return


def check_ship_collision():
    global immunity_counter, game_mode, lives
    if immunity:
        if immunity_counter > 9:
            immunity_counter = 0
        else:
            immunity_counter += 1
    else:
        for astroid in astroids:
            dist = math.hypot(astroid.x - ship.x, astroid.y - ship.y)
            if dist < astroid.diameter + ship.diameter:
                pygame.mixer.Sound.play(boom)
                toggle_immunity()
                if not lives:
                    pygame.mixer.Sound.stop(thrusters)
                    game_mode = "over"
                    mode_select()
                else:
                    lives -= 1


def draw():
    screen.blit(background, (0, 0))

    if game_mode == "playing":
        for astroid in astroids:
            screen.blit(astroid.surf, astroid.rect)

        if not (immunity and immunity_counter < 5):
            screen.blit(ship.surf, ship.rect)

        for shot in shots:
            screen.blit(shot.surf, shot.rect)

        for i in range(1, lives + 1):
            screen.blit(life, (30 * i, 10))

        screen.blit(score_text.surf, score_text.rect)

    else:
        screen.blit(messege_text.surf, messege_text.rect)

    pygame.display.flip()


score_text = Text("SCORE: " + str(score), 20, "topright", (WIDTH - 34, 10))
messege_text = Text("Astroids", 90, "center", (CENTERX, CENTERY))
ship = Ship()
mode_select()
pygame.mixer.music.play(-1)

while running:
    update_input()
    if game_mode == "playing":
        update_ship()
        update_shots()
        update_astroids()
        check_shot_collision()
        check_ship_collision()
    draw()
    pygame.time.Clock().tick(FPS)

pygame.quit()
