#include "Game.h"
#include <fstream>



Game::Game(const std::string& config)
{
	init(config);
}


void Game::init(const std::string& path)
{
	//TODO: read in config file here
	//		use the premade layerConfig, EnemyConfig, BulletConfig variables

	std::fstream newfile;
	int windowWidth = 0, windowHeight = 0, windowFPS = 0, windowMaximised = 0;

	newfile.open(path + "\\GameConfigs.txt", std::ios::in);
	if (newfile.is_open()) {

		std::string arr[20];
		std::string tp;
		while (std::getline(newfile, tp)) {
			int i = 0;
			std::stringstream ssin(tp);
			while (ssin.good()) {
				ssin >> arr[i];
				++i;
			}

			if (arr[0] == "Window") {
				windowWidth = std::stoi(arr[1]);
				windowHeight = std::stoi(arr[2]);
				windowFPS = std::stoi(arr[3]);
				windowMaximised = std::stoi(arr[4]);
			}
			else if (arr[0] == "Font") {
				sf::Font font;
				if (!font.loadFromFile(arr[1])) {
					std::cerr << "Could not load font";
					exit(-1);
				}
				
				//add other params
			}
			else if (arr[0] == "Player") {
				m_playerConfig.SR = stoi(arr[1]);
				m_playerConfig.CR = stoi(arr[2]);
				m_playerConfig.S = stof(arr[3]);
				m_playerConfig.FR = stoi(arr[4]);
				m_playerConfig.FG = stoi(arr[5]);
				m_playerConfig.FB = stoi(arr[6]);
				m_playerConfig.OR = stoi(arr[7]);
				m_playerConfig.OG = stoi(arr[8]);
				m_playerConfig.OB = stoi(arr[9]);
				m_playerConfig.OT = stoi(arr[10]);
				m_playerConfig.V = stoi(arr[11]);
			}
			else if (arr[0] == "Enemy") {
				m_enemyConfig.SR = stoi(arr[1]);
				m_enemyConfig.CR = stoi(arr[2]);
				m_enemyConfig.SMIN = stof(arr[3]);
				m_enemyConfig.SMAX = stof(arr[4]);
				m_enemyConfig.OR = stoi(arr[5]);
				m_enemyConfig.OG = stoi(arr[6]);
				m_enemyConfig.OB = stoi(arr[7]);
				m_enemyConfig.OT = stoi(arr[8]);
				m_enemyConfig.VMIN = stoi(arr[9]);
				m_enemyConfig.VMAX = stoi(arr[10]);
				m_enemyConfig.L = stoi(arr[11]);
				m_enemyConfig.SI = stoi(arr[12]);
			}
			else if (arr[0] == "Bullet") {
				m_bulletConfig.SR = stoi(arr[1]);
				m_bulletConfig.CR = stoi(arr[2]);
				m_bulletConfig.S = stof(arr[3]);
				m_bulletConfig.FR = stoi(arr[4]);
				m_bulletConfig.FG = stoi(arr[5]);
				m_bulletConfig.FB = stoi(arr[6]);
				m_bulletConfig.OR = stoi(arr[7]);
				m_bulletConfig.OG = stoi(arr[8]);
				m_bulletConfig.OB = stoi(arr[9]);
				m_bulletConfig.OT = stoi(arr[10]);
				m_bulletConfig.V = stoi(arr[11]);
				m_bulletConfig.L = stoi(arr[12]);
			}

		}
		newfile.close(); 
	}
	m_window.create(sf::VideoMode(windowWidth, windowHeight), "GeometryWars");
	m_window.setFramerateLimit(windowFPS);
	//m_window.setVisible(windowMaximised);
	//fullscreen mode or not
}

void Game::run()
{
	spawnPlayer();

	//TODO: add pause functionality in here
	//		some systems should function while paused (rendering)
	//		some shouldnt (movement / input)
	while (m_running)
	{
		m_entities.update();

		//sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
		sRender();

		//increment current frame
		//may need to be moved when pause implemented
		m_currentFrame++;
	}
}

void Game::setPaused(bool paused)
{
	//TODO
}

void Game::sMovement()
{
	//TODO: implement all entity movement in this function
	//		should read the m_player->cInput component to determine if the player is moving

	Vec2 playerVelocity = Vec2(0,0);
	if (m_player->cInput->left) {
		playerVelocity.x -= m_playerConfig.S;
	}
	if (m_player->cInput->right) {
		playerVelocity.x += m_playerConfig.S;
	}
	if (m_player->cInput->up) {
		playerVelocity.y -= m_playerConfig.S;
	}
	if (m_player->cInput->down) {
		playerVelocity.y += m_playerConfig.S;
	}

	m_player->cTransform->velocity = playerVelocity;

	for (auto e : m_entities.getEntities()) {
		e->cTransform->pos += e->cTransform->velocity;
	}
	//Sample movement speed update
	//check collisions for entities so they dont leave the window box
}

void Game::sUserInput()
{
	//TODO only setting players input cmponent variables
	//no movement logic
	//movement system will read variables set in this function

	sf::Event event;

	while (m_window.pollEvent(event)) 
	{
		if (event.type == sf::Event::Closed) {
			m_running = false;
		}

		if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
				case sf::Keyboard::W:
					m_player->cInput->up = true;
					break;
				case sf::Keyboard::S:
					m_player->cInput->down = true;
					break;
				case sf::Keyboard::A:
					m_player->cInput->left = true;
					break;
				case sf::Keyboard::D:
					m_player->cInput->right = true;
					break;
			}
		}

		if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code) {
			case sf::Keyboard::W:
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = false;
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				//event.mouseButton.x;
				//event.mouseButton.y;
				//call spawn bullet here
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
			}
			if (event.mouseButton.button == sf::Mouse::Right) {
				//event.mouseButton.x;
				//event.mouseButton.y;
				//call specialweapon
			}
		}
	}
}

void Game::sLifespan()
{
	for (auto e : m_entities.getEntities()) 
	{
		if (!e->cLifespan) { continue; }
		//e->cLifespan->remaining--;
		//auto color = e->cShape->circle.getFillColor();
		//color.a--;
	}
	//TODO: 
	// for all entities
	//if it has a lifespan and is alive scale its alpha channel properly
	//if it has lifespan and its time is up, destroy it
}

void Game::sRender()
{
	//TODO: 
	m_window.clear();

	for (auto entity : m_entities.getEntities()) {
		entity->cShape->circle.setPosition(entity->cTransform->pos.x, entity->cTransform->pos.y);
		
		if (entity->tag() == EntityTag::enemy || entity->tag() == EntityTag::smallEnemy || entity->tag() == EntityTag::player) {
			entity->cTransform->angle += 0.01f;
			entity->cShape->circle.setRotation(entity->cTransform->angle);
		}
		m_window.draw(entity->cShape->circle);
	}
	m_window.display();
}

void Game::sEnemySpawner()
{
	//TODO: 
	//(use m_currentFrame-m_lastEnemySpawnTime) to determine
	//how long it has been since the last enemy spawneed
	if (m_currentFrame - m_lastEnemySpawnTime >= m_enemyConfig.SI) {
		spawnEnemy();
	}
		
}

void Game::sCollision()
{
	for (auto p : m_entities.getEntities(EntityTag::player))
	{
		for (auto e : m_entities.getEntities(EntityTag::enemy)) {
			float dist = p->cTransform->pos.dist(e->cTransform->pos);
			p->cCollision->radius;
		}
		if (p->cTransform->pos.x > (m_window.getSize().x - m_playerConfig.CR)) {
			p->cTransform->pos.x = m_window.getSize().x - m_playerConfig.CR;
		}
		else if (p->cTransform->pos.x < (m_playerConfig.CR)) {
			p->cTransform->pos.x = m_playerConfig.CR;
		}
		if (p->cTransform->pos.y > m_window.getSize().y - m_playerConfig.CR) {
			p->cTransform->pos.y = m_window.getSize().y - m_playerConfig.CR;
		}else if(p->cTransform->pos.y < (m_playerConfig.CR)) {
			p->cTransform->pos.y = m_playerConfig.CR;
		}
	}
	for (auto p : m_entities.getEntities(EntityTag::bullet))
	{
		for (auto e : m_entities.getEntities(EntityTag::enemy)) {
			float dist = p->cTransform->pos.dist(e->cTransform->pos);
			p->cCollision->radius;
		}
	}

	//for (auto p : m_entities.getEntities(EntityTag::enemy)) {
	//	if (p->cTransform->pos.x > (m_window.getSize().x - m_enemyConfig.CR) || p->cTransform->pos.x < (m_enemyConfig.CR)) {
	//		p->cTransform->velocity.x = -p->cTransform->velocity.x;
	//	}
	//	if (p->cTransform->pos.y > (m_window.getSize().y - m_enemyConfig.CR) || p->cTransform->pos.y < (m_enemyConfig.CR)) {
	//		p->cTransform->velocity.y = -p->cTransform->velocity.y;
	//	}
	//}

	//walls collision
	//TODO use collision radius not shape radius
}

void Game::spawnPlayer()
{
	auto entity = m_entities.addEntity(EntityTag::player);
	entity->cTransform = std::make_shared<CTransform>(Vec2(m_window.getSize().x/2, m_window.getSize().y / 2), Vec2(m_playerConfig.S, m_playerConfig.S), 0.0f);
	entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
	entity->cInput = std::make_shared<CInput>();
	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
	m_player = entity;
}

void Game::spawnEnemy()
{
	//TODO: make sure the enemy is spawned properly with the m_enemyConfig variables
	//		the enemy must be spawned commpletely within the bounds of the window
	auto entity = m_entities.addEntity(EntityTag::enemy);
	int spawnY, spawnX;

	//Check enemy doesnt spawn in player collision space
	do  {
		spawnY = rand() % (m_window.getSize().y - m_enemyConfig.CR + 1) + m_enemyConfig.CR;
		spawnX = rand() % (m_window.getSize().x - m_enemyConfig.CR + 1) + m_enemyConfig.CR;
	} while (((spawnY < m_player->cTransform->pos.y + m_playerConfig.CR) && (spawnY > m_player->cTransform->pos.y - m_playerConfig.CR))
		|| ((spawnX < m_player->cTransform->pos.x + m_playerConfig.CR) && (spawnX > m_player->cTransform->pos.x - m_playerConfig.CR)));

	int speed = (rand() % (int)((m_enemyConfig.SMAX - m_enemyConfig.SMIN + 1) * 1000))/1000 + m_enemyConfig.SMIN;
	entity->cTransform = std::make_shared<CTransform>(Vec2(spawnX, spawnY), Vec2(speed, speed), 0.0f);
	entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, rand() % (m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1) + m_enemyConfig.VMIN, sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
		sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
	entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
	//entity->cScore = std::make_shared<CScore>(); what scores?
	entity->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
	//record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(ptr<Entity> e)
{
	//TODO: spawn small enemies at the location of the input enemy e

	size_t vertices = e->cShape->circle.getPointCount();

	for (int i = 0; i < vertices; i++) {
		auto smallEnemy = m_entities.addEntity(EntityTag::smallEnemy);
		e->cShape->circle.getFillColor();
		smallEnemy->cScore = std::make_shared<CScore>(e->cScore->score * 2);
	}

	//when we create the smaller enemy, we have to read the values of the original enemy
	//- spawn a number of small enemies equal to vertices of original enemie
	//- set each small enemy to same color as the original, half the size
	//- small enemies are worth double points of the original enemy
}

void Game::spawnBullet(ptr<Entity> entity, const Vec2& mousePos)
{
	//TODO: implement the spawning of a bullet which travels toward target
	// bullet speed is given as a scalar speed
	auto bullet = m_entities.addEntity(EntityTag::bullet);
	bullet->cTransform = std::make_shared<CTransform>(Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y), Vec2(m_bulletConfig.S, m_bulletConfig.S), 0.0f);
	bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
		sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
	bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
}	

void Game::spawnSpecialWeapon(ptr<Entity> entity)
{
	//TODO:  implement special weapon
}
