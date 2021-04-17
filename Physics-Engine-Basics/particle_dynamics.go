package main

import (
	"fmt"
	"math/rand"
	"time"
)

const (
	NUM_PARTICLES = 1
)

type Vector2 struct {
	x float32
	y float32
}

type Particle struct {
	position Vector2
	velocity Vector2
	mass     float32
}

var particles [NUM_PARTICLES]Particle

func main() {
	runSimulation()
}

func printParticles() {
	for i := 0; i < NUM_PARTICLES; i++ {
		var particle *Particle = &particles[i]

		fmt.Printf("particle[%v] (%.2f, %.2f)\n",
			i, particle.position.x, particle.position.y)

	}
}

func initializeParticles() {
	for i := 0; i < NUM_PARTICLES; i++ {
		particles[i].position = Vector2{rand.Float32(), rand.Float32()}
		particles[i].velocity = Vector2{0, 0}
		particles[i].mass = 1
	}
}

func computeForce(particle *Particle) Vector2 {
	return Vector2{0, particle.mass * -9.81}
}

func runSimulation() {
	var (
		totalSimulationTime float32 = 10
		currentTime         float32 = 0
		dt                  float32 = 1
	)

	initializeParticles()
	printParticles()

	for currentTime < totalSimulationTime {
		time.Sleep(time.Duration(dt) * time.Second)

		for i := 0; i < NUM_PARTICLES; i++ {

			var (
				particle     *Particle = &particles[i]
				force        Vector2   = computeForce(particle)
				acceleration Vector2   = Vector2{
					x: force.x / particle.mass,
					y: force.y / particle.mass,
				}
			)

			particle.velocity.x += acceleration.x * dt
			particle.velocity.y += acceleration.y * dt
			particle.position.x += particle.velocity.x * dt
			particle.position.y += particle.velocity.y * dt

		}

		printParticles()
		currentTime += dt
	}
}
