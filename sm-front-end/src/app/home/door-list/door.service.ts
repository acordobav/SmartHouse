import { Injectable } from '@angular/core';
import { Observable, Subject } from 'rxjs';
import { Door } from 'src/app/models/door.model';

@Injectable({
  providedIn: 'root'
})
export class DoorService {
  private _doors = new Subject<Door[]>();

  constructor() { }

  get doors(): Observable<Door[]> {
    return this._doors.asObservable();
  }

  public fetchDoors() {
    let doors: Door[] = [];

    doors.push(new Door(0, "Puerta Frontal", 0));
    doors.push(new Door(0, "Puerta Trasera", 0));
    doors.push(new Door(0, "Puerta Baño", 1));
    doors.push(new Door(0, "Puerta Cuarto 1", 0));
    doors.push(new Door(0, "Puerta Cuarto 2", 0));

    this._doors.next(doors);
  }
}
