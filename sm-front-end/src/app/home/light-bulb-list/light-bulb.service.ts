import { Injectable } from '@angular/core';
import { Observable, Subject } from 'rxjs';
import { LightBulb } from 'src/app/models/light-bulb.model';

@Injectable({
  providedIn: 'root'
})
export class LightBulbService {
  private _lightBulbs = new Subject<LightBulb[]>();
  private bulbList: LightBulb[] = [];

  constructor() {
    this.bulbList.push(new LightBulb(0, "Luz Cocina", 0));
    this.bulbList.push(new LightBulb(1, "Luz Sala", 0));
    this.bulbList.push(new LightBulb(2, "Luz Baño", 1));
    this.bulbList.push(new LightBulb(3, "Luz Cuarto 1", 0));
    this.bulbList.push(new LightBulb(4, "Luz Cuarto 2", 0));
    this.bulbList.push(new LightBulb(5, "Luz Comedor", 0));
   }

  get lightBulbs(): Observable<LightBulb[]> {
    return this._lightBulbs.asObservable();
  }

  public update() {
    this._lightBulbs.next(this.bulbList);
  }

  public changeState(id: number) {
    if(this.bulbList[id].state) {
      this.bulbList[id].state = 0;
    } else {
      this.bulbList[id].state = 1;
    }
    this.update();
  }

  public turnOff() {
    for (let i = 0; i < this.bulbList.length; i++) {
      this.bulbList[i].state = 0;
    }
    this.update();
  }
  
  public turnOn() {
    for (let i = 0; i < this.bulbList.length; i++) {
      this.bulbList[i].state = 1;
    }
    this.update();
  }
}
