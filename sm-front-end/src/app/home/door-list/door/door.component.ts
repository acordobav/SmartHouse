import { Component, Input, OnInit } from '@angular/core';
import { Door } from 'src/app/models/door.model';

@Component({
  selector: 'app-door',
  templateUrl: './door.component.html',
  styleUrls: ['./door.component.css']
})
export class DoorComponent implements OnInit {
  @Input() door: Door;

  constructor() { }

  ngOnInit(): void {
  }

}
