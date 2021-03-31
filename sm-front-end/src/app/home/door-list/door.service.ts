import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable, Subject } from 'rxjs';
import { Door } from 'src/app/models/door.model';
import { environment } from 'src/environments/environment';

@Injectable({
  providedIn: 'root'
})
export class DoorService {
  private _doors = new Subject<Door[]>();

  constructor(private http: HttpClient) { }

  get doors(): Observable<Door[]> {
    return this._doors.asObservable();
  }

  public fetchDoors() {
    this.http.get<Door[]>(environment.restapiUrl + '/door').subscribe(
      (doors) => {
        this._doors.next(doors);
      }
    );
  }
}
