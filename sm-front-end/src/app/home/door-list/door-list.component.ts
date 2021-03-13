import { Component, OnDestroy, OnInit } from '@angular/core';
import { NgbActiveModal } from '@ng-bootstrap/ng-bootstrap';
import { Subscription } from 'rxjs';
import { Door } from 'src/app/models/door.model';
import { DoorService } from './door.service';

@Component({
  selector: 'app-door-list',
  templateUrl: './door-list.component.html',
  styleUrls: ['./door-list.component.css']
})
export class DoorListComponent implements OnInit, OnDestroy {
  private subDoor: Subscription;
  public doors: Door[] = [];

  constructor(private doorService: DoorService,
    public activeModal: NgbActiveModal) { }

  ngOnInit(): void {
    this.subDoor = this.doorService.doors.subscribe(
      (doors) => {
        this.doors = doors;
      }
    );

    this.doorService.fetchDoors();
  }

  ngOnDestroy(): void {
    this.subDoor.unsubscribe();
  }

}
