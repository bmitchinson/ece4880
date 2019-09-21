import { h, Component } from 'preact';
import { database } from '../firebase/firebase';

import style from './temp.scss';

const tempRef = database.collection('temps');
const tempObj = tempRef.orderBy('time', 'desc').limit(1);

interface MyProps {}
interface MyState {
    displayTemp: number;
}

export default class Temp extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            displayTemp: 0
        };
    }

    componentDidMount() {
        tempObj.onSnapshot(snapShot => {
            snapShot.forEach(doc => {
                const temp = doc.data().temp.toFixed(1);
                this.setState({ displayTemp: temp });
            });
        });
    }

    render() {
        return (
            <div>
                <header class={style.TempLabel}>Current Temp:</header>
                <p class={style.Temp}>{this.state.displayTemp}°C</p>
            </div>
        );
    }
}
