import { h, Component } from 'preact';
import { auth } from '../firebase/firebase';

import Guard from './guard/guard';
import Form from './form/form';

interface MyProps {}
interface MyState {
    currentUser: any;
}

export default class Controls extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            currentUser: null
        };
    }

    componentDidMount() {
        auth.onAuthStateChanged(currentUser => {
            this.setState({ currentUser });
            // TODO: add callback that gets auth status + sets userIsNotAuth
        });
    }

    render() {
        const user = this.state.currentUser;
        const userIsNotAuth = false; // TODO: this.state.userIsNotAuth
        return user ? <Form userIsNotAuth={userIsNotAuth} /> : <Guard />;
    }
}
