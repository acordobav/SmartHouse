import { Component, OnDestroy, OnInit } from '@angular/core';
import { Subscription } from 'rxjs';
import { Door } from 'src/app/models/door.model';
import { LightBulb } from 'src/app/models/light-bulb.model';
import { DoorService } from '../door-list/door.service';
import { LightBulbService } from '../light-bulb-list/light-bulb.service';

@Component({
  selector: 'app-interactive-map',
  templateUrl: './interactive-map.component.svg',
  styleUrls: ['./interactive-map.component.css']
})
export class InteractiveMapComponent implements OnInit, OnDestroy {
  private doorClosedColor = '#2b110000';
  private doorOpenColor = '#2b1100';

  private onBulb = "#f5f542";
  private offBulb = "#00ffff";

  backDoorOpen: string = this.doorClosedColor;
  frontDoorOpen: string = this.doorClosedColor;
  room1DoorOpen: string = this.doorClosedColor;
  room2DoorOpen: string = this.doorClosedColor;
  bathroomDoorOpen: string = this.doorClosedColor;

  backDoorClosed: string = this.doorClosedColor;
  frontDoorClosed: string = this.doorClosedColor;
  room1DoorClosed: string = this.doorClosedColor;
  room2DoorClosed: string = this.doorClosedColor;
  bathroomDoorClosed: string = this.doorClosedColor;

  livingroomBulb: string = this.offBulb;
  kitchenBulb: string = this.offBulb;
  bathroomBulb: string = this.offBulb;
  room1Bulb: string = this.offBulb;
  room2Bulb: string = this.offBulb;
  dinningroomBulb: string = this.offBulb;

  private doorSub: Subscription;
  private lightBulbSub: Subscription;

  constructor(private doorService: DoorService,
    private lightBulbService: LightBulbService) { }

  ngOnInit(): void {
    // Suscripcion a la lista de puertas
    this.doorSub = this.doorService.doors.subscribe(
      (doors) => {
        this.processDors(doors);
      }
    )

    // Suscripcion a la lista de bombillos
    this.lightBulbSub = this.lightBulbService.lightBulbs.subscribe(
      (lightBulbs) => {
        this.processLightBulbs(lightBulbs);
      }
    );

    this.doorService.fetchDoors();
    this.lightBulbService.update();
  }

  ngOnDestroy(): void {
    this.doorSub.unsubscribe();
    this.lightBulbSub.unsubscribe();
  }

  private processDors(doors: Door[]) {
    // Se recorre la lista de puertas
    for (let i = 0; i < doors.length; i++) {
      const door: Door = doors[i];
      this.setDoorColor(door);
    }
  }

  private setDoorColor(door: Door) {
    const openColor = this.doorOpenColor;
    const closedColor = this.doorClosedColor;

    switch (door.id) {
      case 0: // Puerta frontal
        this.frontDoorOpen = door.state ? openColor : closedColor;
        this.frontDoorClosed = door.state ? closedColor : openColor;
        break;

      case 1: // Puerta trasera
        this.backDoorOpen = door.state ? openColor : closedColor;
        this.backDoorClosed = door.state ? closedColor : openColor;
        break;

      case 2: // Puerta banio
        this.bathroomDoorOpen = door.state ? openColor : closedColor;
        this.bathroomDoorClosed = door.state ? closedColor : openColor;
        break;

      case 3: // Puerta cuarto 1
        this.room1DoorOpen = door.state ? openColor : closedColor;
        this.room1DoorClosed = door.state ? closedColor : openColor;
        break;

      case 4: // Puerta cuarto 2
        this.room2DoorOpen = door.state ? openColor : closedColor;
        this.room2DoorClosed = door.state ? closedColor : openColor;
        break;

      default:
        break;
    }
  }

  private processLightBulbs(bulbs: LightBulb[]) {
    for (let i = 0; i < bulbs.length; i++) {
      const bulb: LightBulb = bulbs[i];
      this.setBulbColor(bulb);
    }
  }

  private setBulbColor(bulb: LightBulb) {
    switch (bulb.id) {
      case 0: // Cocina
        this.kitchenBulb = bulb.state ? this.onBulb : this.offBulb;
        break;

      case 1: // Sala
        this.livingroomBulb = bulb.state ? this.onBulb : this.offBulb;
        break;

      case 2: // Banio
        this.bathroomBulb = bulb.state ? this.onBulb : this.offBulb;
        break;
      
      case 3: // Cuarto 1
        this.room1Bulb = bulb.state ? this.onBulb : this.offBulb;
        break;
      
      case 4: // Cuarto 2
        this.room2Bulb = bulb.state ? this.onBulb : this.offBulb;
        break;
      
      case 5: // Comedor
        this.dinningroomBulb = bulb.state ? this.onBulb : this.offBulb;
        break;

      default:
        break;
    }
  }

  changeBulbState(id: number) {
    this.lightBulbService.changeState(id);
  }
}
