import { h, Component } from 'preact';
import { database } from '../firebase/firebase';
import { TrendChart, TimestampArray } from 'preact-charts';

interface MyProps {}
interface MyState {
    dataArray: TimestampArray;
}

export default class Graph extends Component<MyProps, MyState> {
    constructor() {
        super();
        const dataArray = [];
        // for (let i = 0; i < 5; i += 1) {
        //     const now = new Date();
        //     dataArray.push({
        //         timestamp: now.setSeconds(now.getSeconds() - i),
        //         temp: Math.random() * 60 - 10
        //     });
        // }
        this.state = {
            dataArray
        };
    }

    // componentDidMount() {}
    // Fetch 300 seconds, reg collection onChange

    // collection on change: grab 1 and add it to stack

    render() {
        const { dataArray } = this.state;
        return (
            <TrendChart
                name="Temps"
                x="timestamp"
                y="temp"
                data={dataArray}
                margin={{ top: 60, right: 80, left: 80, bottom: 60 }}
                axisControl={false}
            />
        );
    }
}
