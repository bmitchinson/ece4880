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
        for (let i = 0; i < 100; i += 1) {
            const time = new Date();
            time.setSeconds(time.getSeconds() - i);
            dataArray.push({
                timestamp: time,
                temp: Math.random() * 60 - 10
            });
        }
        dataArray.map(val => {
            if (val.temp > 50) {
                val.temp = 50;
            }
            if (val.temp < 0) {
                val.temp = 0;
            }
            return val;
        });
        this.state = { dataArray };
    }

    componentDidMount() {
        setInterval(this.addNewPoint, 1000);
    }

    addNewPoint = () => {
        const dataArrayEdit = this.state.dataArray;
        const time = new Date();
        dataArrayEdit.push({
            timestamp: time,
            temp: Math.random() * 60 - 10
        });
        dataArrayEdit.sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
        dataArrayEdit.splice(0, 1);
        this.setState({ dataArray: dataArrayEdit });
    };
    // Fetch 300 seconds, reg collection onChange

    // collection on change: grab 1 and add it to stack

    render() {
        const { dataArray } = this.state;
        console.log('given:', dataArray);
        dataArray[0].temp = 0;
        dataArray[4].temp = 50;
        return (
            <TrendChart
                lineColour="#90d7c2"
                name="Temps"
                x="timestamp"
                y="temp"
                data={dataArray}
                margin={{ top: 60, right: 80, left: 80, bottom: 60 }}
                axisControl={false}
                tooltip={false}
            />
        );
    }
}
// import { h } from 'preact';

// const Graph = () => <h2>Graph</h2>;

// export default Graph;
