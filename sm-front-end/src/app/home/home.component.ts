import { Component, OnInit } from '@angular/core';
import { DoorService } from './door-list/door.service';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css']
})
export class HomeComponent implements OnInit {

  constructor(private doorService: DoorService) { 
    this.doorService.fetchDoors();
  }

  ngOnInit(): void {
  }

}
