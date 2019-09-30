import { h, Component } from 'preact';
import { database } from '../firebase/firebase';
import { TrendChart, TimestampArray } from 'preact-charts';

interface MyProps {}
interface MyState {
    dataArray: TimestampArray;
    dataCoverArray: TimestampArray;
}

export default class Graph extends Component<MyProps, MyState> {
    constructor() {
        super();
        const dataArray = getFakeDataOne();
        const dataCoverArray = getCoverArray(dataArray);
        boundedDataArray(dataArray);
        boundedDataArray(dataCoverArray);

        this.state = { dataArray, dataCoverArray };
    }

    componentDidMount() {
        // setTimeout(this.addNewPoint, 3000);
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
        const dataCoverArrayEdit = getCoverArray(dataArrayEdit);

        this.setState({
            dataArray: dataArrayEdit,
            dataCoverArray: dataCoverArrayEdit
        });
    };

    render() {
        const { dataArray, dataCoverArray } = this.state;
        dataArray[0].temp = 0;
        dataArray[1].temp = 50;
        // console.log(
        //     'data:',
        //     boundedDataArray(dataArray),
        //     'cover:',
        //     boundedDataArray(dataCoverArray)
        // );
        return (
            <TrendChart
                lineColour="#90d7c2"
                lineColourTwo="#ffffff"
                name="Temps"
                x="timestamp"
                y="temp"
                data={dataArray}
                dataSetTwo={dataCoverArray}
                margin={{ top: 60, right: 80, left: 80, bottom: 60 }}
                axisControl={false}
                tooltip={false}
            />
        );
    }
}

const boundedDataArray = array => {
    array.map(val => {
        if (val.temp > 50) {
            val.temp = 50;
        }
        if (val.temp < 0) {
            val.temp = 0;
        }
        return val;
    });
};

// const boundedDataArray = array => {
//     return array.map(val => {
//         if (val.temp > 50) {
//             return 50;
//         }
//         if (val.temp < 0) {
//             return 0;
//         }
//         return val;
//     });
// };

const getFakeDataOne = () => {
    const dataArray = [];
    for (let i = 0; i < 33; i += 1) {
        const time = new Date();
        time.setSeconds(time.getSeconds() - i);
        dataArray.push({
            timestamp: time,
            temp: Math.random() * 50
        });
    }
    for (let i = 33; i < 66; i += 1) {
        const time = new Date();
        time.setSeconds(time.getSeconds() - i);
        dataArray.push({
            timestamp: time,
            temp: -100
        });
    }
    for (let i = 66; i < 99; i += 1) {
        const time = new Date();
        time.setSeconds(time.getSeconds() - i);
        dataArray.push({
            timestamp: time,
            temp: Math.random() * 50
        });
    }
    dataArray.sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
    return dataArray;
};

const getCoverArray = dataArray => {
    const dataCoverArray = [];
    dataCoverArray.push({
        timestamp: dataArray[0].timestamp,
        temp: 0
    });
    dataCoverArray.push({
        timestamp: dataArray[dataArray.length - 1].timestamp,
        temp: 0
    });
    dataArray.forEach((ele, index, dataArray) => {
        if (
            index !== 0 &&
            ele.temp === -100 &&
            dataArray[index - 1].temp !== -100
        ) {
            dataCoverArray.push({
                timestamp: dataArray[index - 1].timestamp.setMilliseconds(
                    dataArray[index - 1].timestamp.getMilliseconds() - 2
                ),
                temp: 0
            });
            dataCoverArray.push({
                timestamp: dataArray[index - 1].timestamp,
                temp: dataArray[index - 1].temp
            });
            dataCoverArray.push({
                timestamp: ele.timestamp,
                temp: 0
            });
        } else if (
            index !== dataArray.length - 1 &&
            ele.temp === -100 &&
            dataArray[index + 1].temp !== -100
        ) {
            dataCoverArray.push({
                timestamp: ele.timestamp,
                temp: 0
            });
            dataCoverArray.push({
                timestamp: dataArray[index + 1].timestamp,
                temp: dataArray[index + 1].temp
            });
            dataCoverArray.push({
                timestamp: dataArray[index + 1].timestamp.setMilliseconds(
                    dataArray[index + 1].timestamp.getMilliseconds() + 2
                ),
                temp: 0
            });
        }
    });
    dataCoverArray.sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
    return dataCoverArray;
};
