import pygame, sys
from pygame.locals import *
import random

'''飞机躲避导弹'''

# 根据概率随机选取
def choice(seq, prob):
    p = random.random()
    for i in range(len(seq)):
        if sum(prob[:i]) < p < sum(prob[:i+1]):
            return seq[i]


# 玩家
class PlayerSprite(pygame.sprite.Sprite):
    speed = 5

    def __init__(self):
        super().__init__()
        self.image = pygame.image.load('jet.png').convert() # load函数，返回一个 Surface 对象
        self.image.set_colorkey((255,255,255), RLEACCEL)
        self.rect = self.image.get_rect()

    def update(self, key):
        if key[K_UP]:
            self.rect.move_ip(0, -self.speed)
        if key[K_DOWN]:
            self.rect.move_ip(0, self.speed)
        if key[K_LEFT]:
            self.rect.move_ip(-self.speed, 0)
        if key[K_RIGHT]:
            self.rect.move_ip(self.speed, 0)

        # 限定player在屏幕中
        if self.rect.left < 0:
            self.rect.left = 0
        elif self.rect.right > 800:
            self.rect.right = 800
        if self.rect.top <= 0:
            self.rect.top = 0
        elif self.rect.bottom >= 600:
            self.rect.bottom = 600

# 敌人
class EnemySprite(pygame.sprite.Sprite):
    speed = choice([1,3,5], [0.5, 0.4, 0.1])

    def __init__(self):
        super().__init__()
        self.image = pygame.image.load('missile.png').convert()
        self.image.set_colorkey((255, 255, 255), RLEACCEL)
        self.rect = self.image.get_rect(center=(820, random.randint(0, 600)))
        #self.speed = random.randint(5, 20)

    def update(self):
        self.rect.move_ip(-self.speed, 0) # 从右向左
        if self.rect.right < 0:
            self.kill() # Sprite 的内建方法

# 白云
class CloudSprite(pygame.sprite.Sprite):
    speed = choice([1,2], [0.8, 0.2])

    def __init__(self):
        super().__init__()
        self.image = pygame.image.load('cloud.png').convert()
        self.image.set_colorkey((0,0,0),RLEACCEL)
        self.rect = self.image.get_rect(
            center = (random.randint(820,900),random.randint(0,600))
        )

    def update(self):
        self.rect.move_ip(-self.speed,0)
        if self.rect.right < 0:
            self.kill()


# 背景
class BackgroundSprite(pygame.sprite.Sprite):

    def __init__(self, size):
        super().__init__()
        self.image = pygame.Surface(size)  # 实际上是surf，为了统一写成image
        self.image.fill((135, 206, 250)) # 浅蓝色
        self.rect = pygame.Rect(0, 0, *size)

    def update(self):
        pass


class Game():

    def __init__(self):
        # 游戏初始化
        pygame.init()

        # 屏幕大小及屏幕对象
        self.size = self.width, self.height = 800, 600
        self.screen = pygame.display.set_mode(self.size)

        pygame.display.set_caption("Pygame 2D RPG !")


        # 两个自定义事件
        self.ADDENEMY = pygame.USEREVENT + 1      # 事件本质上就是整数常量。比 USEREVENT 小的数值已经对应内置事件，因此任何自定义事件都必须比 USEREVENT 大）
        pygame.time.set_timer(self.ADDENEMY, 250) # 每隔 250 毫秒(四分之一秒) 触发
        self.ADDCLOUD = pygame.USEREVENT + 2
        pygame.time.set_timer(self.ADDCLOUD, 1000)

        # 两个精灵对象
        self.background = BackgroundSprite(self.size)
        self.player = PlayerSprite()

        # 三个精灵组
        self.enemies = pygame.sprite.Group()
        self.clouds = pygame.sprite.Group()
        self.all_sprites = pygame.sprite.Group()

        self.all_sprites.add(self.player)


    def run(self):
        # 窗口主循环
        while True:
            # 遍历事件队列
            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()
                    sys.exit()
                elif event.type == KEYDOWN:
                    if event.key == K_ESCAPE:
                        pygame.quit()
                        sys.exit()
                elif event.type == self.ADDENEMY: # 自定义事件
                    new_enemy = EnemySprite()
                    self.enemies.add(new_enemy)
                    self.all_sprites.add(new_enemy)
                elif event.type == self.ADDCLOUD: # 自定义事件
                    new_cloud = CloudSprite()
                    self.clouds.add(new_cloud)
                    self.all_sprites.add(new_cloud)

            # 背景
            self.screen.blit(self.background.image, self.background.rect)

            # 获取按键
            key = pygame.key.get_pressed()

            # 更新精灵（组）
            self.player.update(key)
            self.enemies.update()
            self.clouds.update()

            # 放置精灵
            for sprite in self.all_sprites:
                self.screen.blit(sprite.image, sprite.rect)

            # 碰撞检测（灵魂所在）
            if pygame.sprite.spritecollideany(self.player, self.enemies):
                self.player.kill()
                print('发生碰撞！')

            # 重绘界面
            pygame.display.flip()



if __name__ == '__main__':
    Game().run()


